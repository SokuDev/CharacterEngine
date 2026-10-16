//
// Created by PinkySmile on 02/03/2026.
//

#ifndef CHARACTERENGINE_CHARACTERENGINE_HPP
#define CHARACTERENGINE_CHARACTERENGINE_HPP


#include <SokuLib.hpp>

#define assert_exp_mt(expr) do { if (!(expr)) { \
	std::string s = "Debug assertion \"" #expr "\" failed at " __FILE__ " line " + std::to_string(__LINE__) + " in " + __FUNCSIG__ + "."; \
	MessageBoxA(SokuLib::window, s.c_str(), "Fatal Error", MB_ICONERROR); \
	throw std::runtime_error(s); \
} } while(0)
#define assert_exp_ma(expr) do { if (!(expr)) { \
	std::string s = "Debug assertion \"" #expr "\" failed at " __FILE__ " line " + std::to_string(__LINE__) + " in " + __FUNCSIG__ + "."; \
	MessageBoxA(SokuLib::window, s.c_str(), "Fatal Error", MB_ICONERROR); \
	abort(); \
} } while(0)
#define assert_exp_m(expr) do { if (!(expr)) { \
	std::string s = "Debug assertion \"" #expr "\" failed at " __FILE__ " line " + std::to_string(__LINE__) + " in " + __FUNCSIG__ + "."; \
	MessageBoxA(SokuLib::window, s.c_str(), "Fatal Error", MB_ICONERROR); \
} } while(0)
#define assert_exp_mr(expr, e) do { if (!(expr)) { \
	std::string s = "Debug assertion \"" #expr "\" failed at " __FILE__ " line " + std::to_string(__LINE__) + " in " + __FUNCSIG__ + "."; \
	MessageBoxA(SokuLib::window, s.c_str(), "Fatal Error", MB_ICONERROR); \
	return e; \
} } while(0)
#define assert_exp_t(expr) do { if (!(expr)) { \
	throw std::runtime_error("Debug assertion \"" #expr "\" failed at " __FILE__ " line " + std::to_string(__LINE__) + " in " + __FUNCSIG__ + "."); \
} } while(0)
#define ABI_VERSION 1
#define ABIS_SUPPORTED {1}

struct CharacterEngineABI {
	unsigned version;
	unsigned supportedVersions[32];
	size_t (*getCharacterSize)(int id);
	size_t (*getObjectSize)(int id);
	SokuLib::v2::Player *(*createCharacter)(int id, SokuLib::PlayerInfo &info);
	void (*destroyCharacter)(SokuLib::v2::Player *ptr);
};


#endif //CHARACTERENGINE_CHARACTERENGINE_HPP