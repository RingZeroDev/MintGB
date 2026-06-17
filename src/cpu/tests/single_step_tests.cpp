#include "cpu/cpu.hpp"

#include "json.hpp"

#include <fstream>
#include <iostream>
#include <string>
#include <filesystem>
#include <format>

using Json = nlohmann::json;

class SingleStepTests : private CPU {
    private:
        std::array<uint8_t, 0x10000> memory{};
        MemoryBus bus { memory };

        CPU cpu { bus };

        Json parseJson(const char* path) {
            std::ifstream f(path);
            Json data = Json::parse(f);

            return data;
        }

        static constexpr std::array<uint8_t, 12> ILLEGAL_INSTRUCTIONS = {
            0xCB, 0xD3, 0xDB, 0xDD, 0xE3, 0xE4, 0xEB, 0xEC, 0xED, 0xF4, 0xFC, 0xFD
        };

        static constexpr std::array<const char*, 3> accessLut {{
            "r-m",
            "-wm",
            "---"
        }};

        void setState(const Json& state) {
            a = state["a"].get<uint8_t>();
            b = state["b"].get<uint8_t>();
            c = state["c"].get<uint8_t>();
            d = state["d"].get<uint8_t>();
            e = state["e"].get<uint8_t>();
            f = state["f"].get<uint8_t>();
            h = state["h"].get<uint8_t>();
            l = state["l"].get<uint8_t>();
            
            pc = state["pc"].get<uint16_t>();
            sp = state["sp"].get<uint16_t>();
            
            ime = state["ime"].get<int>() == 1;
            imePending = state["ie"].get<int>() == 1;

            for (const Json& ramValue : state["ram"]) {
                uint16_t addr = ramValue[0];
                uint8_t value = ramValue[1];
                memory[addr] = value;
            }
        }

        Json getCPUState(const Json& state) {
            Json cpuState = {
                {"a", a},
                {"b", b},
                {"c", c},
                {"d", d},
                {"e", e},
                {"f", f},
                {"h", h},
                {"l", l},
                {"pc", pc},
                {"sp", sp},
                {"ime", ime ? 1 : 0},
                {"ram", Json::array()}
            };

            for (const Json& ramValue : state["ram"]) {
                uint16_t addr = ramValue[0];
                cpuState["ram"].push_back({addr, memory[addr]});
            }

            return cpuState;
        }

        bool checkState(const Json& state) {
            bool passed = true;

            passed = passed && (a == state["a"].get<uint8_t>());
            passed = passed && (b == state["b"].get<uint8_t>());
            passed = passed && (c == state["c"].get<uint8_t>());
            passed = passed && (d == state["d"].get<uint8_t>());
            passed = passed && (e == state["e"].get<uint8_t>());
            passed = passed && (f == state["f"].get<uint8_t>());
            passed = passed && (h == state["h"].get<uint8_t>());
            passed = passed && (l == state["l"].get<uint8_t>());
            
            passed = passed && (pc == state["pc"].get<uint16_t>());
            passed = passed && (sp == state["sp"].get<uint16_t>());
            
            passed = passed && (ime == state["ime"].get<int>() == 1);

            for (const Json& ramValue : state["ram"]) {
                uint16_t addr = ramValue[0];
                uint8_t value = ramValue[1];
                passed = passed && (memory[addr] == value);
            }

            return passed;
        }

        Json getCycles() {
            Json output = Json::array();

            for (const BusAccess& access : bus.accesses) {
                output.push_back({ access.addr, access.value, accessLut[static_cast<int>(access.type)] });
            }

            return output;
        }

        bool checkCycles(const Json& state) {
            for (auto it = state.begin(); it != state.end(); ++it) {
                const Json& cycle = *it;
                const BusAccess& access = bus.accesses[std::distance(state.begin(), it)];

                uint16_t addr = cycle[0].get<uint16_t>();
                uint8_t value = cycle[1].get<uint8_t>();
                std::string_view accessType = cycle[2].get<std::string_view>();
                
                bool pinsRight = access.addr == addr && access.value == value;
                bool accessTypeRight = accessLut[static_cast<int>(access.type)] == accessType;

                if (!pinsRight || !accessTypeRight) return false;
            }

            return true;
        }

        void runTest(const Json& test) {
            setState(test["initial"]);
            step();
            
            if (!checkState(test["final"]) || !checkCycles(test["cycles"])) {
                std::cerr << "Test " << test["name"] << " failed!" << std::endl;
                std::cerr << "Initial CPU State: " << test["initial"].dump(4) << std::endl;
                std::cerr << "Observed CPU State: " << getCPUState(test["final"]).dump(4) << std::endl; 
                std::cerr << "Expected CPU State: " << test["final"].dump(4) << std::endl;
                std::cerr << "Observed CPU Cycles: " << getCycles().dump(4) << std::endl;
                std::cerr << "Expected CPU Cycles: " << test["cycles"].dump(4) << std::endl;
                std::exit(EXIT_FAILURE);
            }
        }

        void testInstruction(const char* path) {
            std::filesystem::path p(path);
            std::cout << "Beginning instruction test: " << p.stem() << std::endl;

            Json data = parseJson(path);

            for (const Json& test : data) {
                bus.accesses.clear();
                runTest(test);
            }

            std::cout << "Test passed!" << std::endl;
        }

    public:
        SingleStepTests() : CPU(bus) {}

        void runSuite(const char* path) {
            // unprefixed instructions
            for (int i = 0x00; i <= 0xFF; i++) {
                if (std::ranges::find(ILLEGAL_INSTRUCTIONS, i) != ILLEGAL_INSTRUCTIONS.end()) {
                    continue;
                }

                std::filesystem::path folder(path);
                std::filesystem::path file(std::format("{:02X}.json", i));
                std::filesystem::path final = folder /= file;

                testInstruction(final.string().c_str());
            }

            // CB instructions
            for (int i = 0x00; i <= 0xFF; i++) {
                std::filesystem::path folder(path);
                std::filesystem::path file(std::format("CB {:02X}.json", i));
                std::filesystem::path final = folder /= file;

                testInstruction(final.string().c_str());
            }
        }
};

int main(int argc, char* argv[]) {
    SingleStepTests tests;

    const char* path = argv[1];

    tests.runSuite(path);
    return EXIT_SUCCESS;
}