#include "cpu/cpu.hpp"

#include "json.hpp"

#include <fstream>
#include <iostream>
#include <string>
#include <filesystem>
#include <format>

using json = nlohmann::json;

class SingleStepTests {
    private:
        std::array<uint8_t, 0x10000> memory{};
        MemoryBus bus { memory };

        CPU cpu { bus };

        json parseJson(const char* path) {
            std::ifstream f(path);
            json data = json::parse(f);

            return data;
        }

        void setState(const json& state) {
            cpu.a = state["a"].get<uint8_t>();
            cpu.b = state["b"].get<uint8_t>();
            cpu.c = state["c"].get<uint8_t>();
            cpu.d = state["d"].get<uint8_t>();
            cpu.e = state["e"].get<uint8_t>();
            cpu.f = state["f"].get<uint8_t>();
            cpu.h = state["h"].get<uint8_t>();
            cpu.l = state["l"].get<uint8_t>();
            
            cpu.pc = state["pc"].get<uint16_t>();
            cpu.sp = state["sp"].get<uint16_t>();
            
            cpu.ime = state["ime"].get<int>() == 1;
            cpu.imePending = state["ie"].get<int>() == 1;

            for (const json& ramValue : state["ram"]) {
                uint16_t addr = ramValue[0];
                uint8_t value = ramValue[1];
                memory[addr] = value;
            }
        }

        json getCPUState(const json& state) {
            json cpuState = {
                {"a", cpu.a},
                {"b", cpu.b},
                {"c", cpu.c},
                {"d", cpu.d},
                {"e", cpu.e},
                {"f", cpu.f},
                {"h", cpu.h},
                {"l", cpu.l},
                {"pc", cpu.pc},
                {"sp", cpu.sp},
                {"ime", cpu.ime ? 1 : 0},
                {"ram", json::array()}
            };

            for (const json& ramValue : state["ram"]) {
                uint16_t addr = ramValue[0];
                cpuState["ram"].push_back({addr, memory[addr]});
            }

            return cpuState;
        }

        bool checkState(const json& state) {
            bool passed = true;

            passed = passed && (cpu.a == state["a"].get<uint8_t>());
            passed = passed && (cpu.b == state["b"].get<uint8_t>());
            passed = passed && (cpu.c == state["c"].get<uint8_t>());
            passed = passed && (cpu.d == state["d"].get<uint8_t>());
            passed = passed && (cpu.e == state["e"].get<uint8_t>());
            passed = passed && (cpu.f == state["f"].get<uint8_t>());
            passed = passed && (cpu.h == state["h"].get<uint8_t>());
            passed = passed && (cpu.l == state["l"].get<uint8_t>());
            
            passed = passed && (cpu.pc == state["pc"].get<uint16_t>());
            passed = passed && (cpu.sp == state["sp"].get<uint16_t>());
            
            passed = passed && (cpu.ime == state["ime"].get<int>() == 1);

            for (const json& ramValue : state["ram"]) {
                uint16_t addr = ramValue[0];
                uint8_t value = ramValue[1];
                passed = passed && (memory[addr] == value);
            }

            return passed;
        }

        json getCycles() {
            constexpr std::array<const char*, 3> accessLut {{
            "r-m",
            "-wm",
            "---"
            }};

            json output = json::array();

            for (const BusAccess& access : bus.accesses) {
                output.push_back({ access.addr, access.value, accessLut[static_cast<int>(access.type)] });
            }

            return output;
        }

        bool checkCycles(const json& state) {
            constexpr std::array<const char*, 3> accessLut {{
            "r-m",
            "-wm",
            "---"
            }};

            for (auto it = state.begin(); it != state.end(); ++it) {
                const json& cycle = *it;
                const BusAccess& access = bus.accesses[std::distance(state.begin(), it)];

                if (access.addr != cycle[0].get<uint16_t>() || access.value != cycle[1].get<uint8_t>()) return false;
                if (accessLut[static_cast<int>(access.type)] != cycle[2].get<std::string_view>()) return false;
            }

            return true;
        }

        void runTest(const json& test) {
            setState(test["initial"]);
            cpu.step();
            
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

            json data = parseJson(path);

            for (const json& test : data) {
                bus.accesses.clear();
                runTest(test);
            }

            std::cout << "Test passed!" << std::endl;
        }

    public:
        void runSuite(const char* path) {
            for (int i = 0x00; i <= 0xFF; i++) {
                if (i == 0xCB || i == 0xD3 || i == 0xDB || i == 0xDD || i == 0xE3 || i == 0xE4 || i == 0xEB || i == 0xEC || i == 0xED || i == 0xF4 || i == 0xFC || i == 0xFD) {
                    continue;
                }

                std::filesystem::path folder(path);
                std::filesystem::path file(std::format("{:02X}.json", i));
                std::filesystem::path final = folder /= file;

                testInstruction(final.string().c_str());
            }

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