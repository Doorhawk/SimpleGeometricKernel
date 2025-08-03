#pragma once
#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <stdexcept>
#include <sstream>
#include <functional>
#include <cmath>
#include <chrono>
#include <ctime>
#include <filesystem>

#ifdef MYUNITTESTDLL_EXPORTS
#define UNITTEST_API __declspec(dllexport)
#else
#define UNITTEST_API __declspec(dllimport)
#endif


#define RED_TEXT "\033[1;31m"
#define GREEN_TEXT "\033[1;32m"
#define RESET_COLOR "\033[0m"
#define BLUE_COLOR "\033[0;36m"
#define DARK_GREY "\033[0;90m"

#define EXPECT_THROW_MSG(stmt, exc_type, msg) \
    do {\
        bool caught = false;\
        try {\
            stmt;\
        } catch (const exc_type& e) {\
            if (std::string(e.what()) == msg) {\
                caught = true;\
				ut::TestRegistry::getInstance().getThisTest().addLog("OK " #exc_type " " msg);\
            } else {\
                std::ostringstream oss; \
                oss << "Expected exception message: \"" << msg << "\", but got: \"" << e.what() << "\"";\
				ut::TestRegistry::getInstance().getThisTest().addLog("FAIL " + oss.str());\
                throw std::invalid_argument(oss.str());\
            }\
        } catch (...) {\
			ut::TestRegistry::getInstance().getThisTest().addLog("FAIL Expected exception of type " #exc_type ", but got a different exception.");\
            throw std::invalid_argument("Expected exception of type " #exc_type ", but got a different exception.");\
        }\
        if (!caught) {\
			ut::TestRegistry::getInstance().getThisTest().addLog("FAIL Expected exception of type " #exc_type " was not thrown.");\
            throw std::invalid_argument("Expected exception of type " #exc_type " was not thrown.");\
        }\
    } while (0)

#define EXPECT_THROW(stmt, exc_type) \
    do {\
        bool caught = false;\
        try {\
            stmt;\
        } catch (const exc_type& e) {\
                caught = true;\
				ut::TestRegistry::getInstance().getThisTest().addLog("OK " #exc_type);\
        } catch (...) {\
			ut::TestRegistry::getInstance().getThisTest().addLog("FAIL Expected " #exc_type ", got a different exception.");\
            throw std::invalid_argument("Expected " #exc_type ", got a different exception.");\
        }\
        if (!caught) {\
			ut::TestRegistry::getInstance().getThisTest().addLog("FAIL Expected " #exc_type " was not thrown.");\
            throw std::invalid_argument("Expected " #exc_type " was not thrown.");\
        }\
    } while (0)

// дл€ а и b должны быть определены != и <<
#define EQVAL(a,b)\
	do{\
		if((a)!=(b)) {\
			std::ostringstream oss;\
			oss << #a " != " #b << ", expected: "<<a<<", got "<<b;\
			ut::TestRegistry::getInstance().getThisTest().addLog("FAIL "+oss.str());\
			throw std::invalid_argument(oss.str()); \
		}\
		else{\
			std::ostringstream oss;\
			oss << #a " == " #b << ", "<<a<<"=="<<b;\
			ut::TestRegistry::getInstance().getThisTest().addLog("OK "+oss.str());\
		}\
	}while(0)

#define EQVAL_EPS(a, b, eps) \
    do { \
		if ((eps) <= 0) { \
            throw std::invalid_argument("Epsilon must be positive: " #eps); \
        } \
        if (std::abs((a) - (b)) >= (eps)) { \
            std::ostringstream oss; \
            oss << "Expected: |" #a " - " #b "| < " #eps \
                << ", got: " << std::abs((a) - (b)) << " >= " << (eps); \
			ut::TestRegistry::getInstance().getThisTest().addLog("FAIL "+oss.str());\
            throw std::runtime_error(oss.str()); \
        } \
		else{\
			std::ostringstream oss; \
            oss << " |" #a " - " #b "| < " #eps \
                << ", " << std::abs((a) - (b)) << " < " << (eps); \
			ut::TestRegistry::getInstance().getThisTest().addLog("OK "+oss.str());\
		}\
    } while (0)

#define CHECK(obj)\
	do{\
		if(!(obj)) {\
			ut::TestRegistry::getInstance().getThisTest().addLog("FAIL !" #obj);\
			throw std::invalid_argument("!"#obj); \
		}\
		else{\
			ut::TestRegistry::getInstance().getThisTest().addLog("OK " #obj);\
		}\
	}while(0)


#define TEST(name) \
	void name();\
	struct name##_registrar{\
		name##_registrar() {\
			ut::TestRegistry::getInstance().addTest(ut::Test(#name, name));\
		}\
	} name##_instance;\
	void name()\

#define TEST_GROUP(groupName,testName)\
	void testName();\
	struct testName##_registrar{\
		testName##_registrar() {\
			ut::TestRegistry::getInstance().addTest(ut::Test(#testName, testName,#groupName));\
		}\
	} testName##_instance;\
	void testName()\


#define RUN_ALL_TESTS() ut::TestRegistry::getInstance().runAll();
#define RUN_TESTS_GROUP(name) ut::TestRegistry::getInstance().runGroup(#name);

// создает перечисленные в body один раз обьекты и дальше в тестах можно испольховать эти обьекты
#define DEFINE_FIXATURE(name,body)\
	struct name_fixature {\
		name_fixature() {/*std::cout <<DARK_GREY<< "[SETUP]"<<RESET_COLOR<< " " #name " created\n";*/}\
		~name_fixature() {/*std::cout <<DARK_GREY<< "[TEARDOWN]"<<RESET_COLOR<<" " #name " destroyed\n";*/}\
		body\
	}name;

#define TEST_PARAM_CASE(name, ...) \
    void name##_case(__VA_ARGS__)


#define CONCAT_IMPL(a, b) a##b
#define CONCAT(a, b) CONCAT_IMPL(a, b)
#define UNIQUE_NAME(base) CONCAT(base, __LINE__)

// ћакрос дл€ преобразовани€ аргументов в строку (иначе `,` ломает макрос)
#define STRINGIFY_IMPL(x) #x
#define STRINGIFY(x) STRINGIFY_IMPL(x)

// ћакрос дл€ регистрации теста с параметрами
#define TEST_PARAM(name, ...) \
    void UNIQUE_NAME(name##_test)(); \
    struct UNIQUE_NAME(name##_registrar) { \
        UNIQUE_NAME(name##_registrar)() { \
            ut::TestRegistry::getInstance().addTest(ut::Test(#name "( " STRINGIFY(__VA_ARGS__) " )", UNIQUE_NAME(name##_test))); \
        } \
    } UNIQUE_NAME(name##_instance); \
    void UNIQUE_NAME(name##_test)() { \
        name##_case(__VA_ARGS__); \
    }

#define TEST_PARAM_GROUP(group,name, ...) \
    void UNIQUE_NAME(name##_test)(); \
    struct UNIQUE_NAME(name##_registrar) { \
        UNIQUE_NAME(name##_registrar)() { \
            ut::TestRegistry::getInstance().addTest(ut::Test(#name "( " STRINGIFY(__VA_ARGS__) " )", UNIQUE_NAME(name##_test),#group)); \
        } \
    } UNIQUE_NAME(name##_instance); \
    void UNIQUE_NAME(name##_test)() { \
        name##_case(__VA_ARGS__); \
    }

#define TEST_DATA(name,data,param)\
	void name##_test(param);\
	struct name##_registrar {\
		name##_registrar() {\
			ut::TestRegistry::getInstance().addTest(ut::Test(#name,[] {\
				ut::runDataTest(name##_test,data);\
			}));\
		}\
	} name##_instance;\
	void name##_test(param)

#define TEST_DATA_GROUP(group,name,data,param)\
	void name##_test(param);\
	struct name##_registrar {\
		name##_registrar() {\
			ut::TestRegistry::getInstance().addTest(ut::Test(#name,[] {\
				ut::runDataTest(name##_test,data);\
			},#group));\
		}\
	} name##_instance;\
	void name##_test(param)

#define TEST_LOG_ON() ut::TestRegistry::getInstance().TestLogOnOff(true)
#define TEST_LOG_OFF() ut::TestRegistry::getInstance().TestLogOnOff(false);

namespace ut {
	class UNITTEST_API Test {
		using TestFun = std::function<void()>;
	public:
		Test(const std::string& name, TestFun fun, const std::string& groupName = "WITHOUTGROUP");
		Test(const Test& other);
		~Test();
		void run();
		const std::string& getName() const;
		const std::string& getError() const;
		const std::string& getGroupName() const;
		bool isPassed() const;
		const std::vector<std::string>& getLog() const;
		void addLog(std::string);
		void clearLog();
	private:
		std::vector<std::string> testLog;
		std::string groupName;
		std::string error;
		std::string name;
		TestFun fun;
		bool passed;

		friend class TestRegistry;
	};
	class SimpleLogger {
	public:
		std::ofstream out;
		SimpleLogger();
		~SimpleLogger();
		std::string getCurrentTime();
		void createLogDirectory();
	};
	class UNITTEST_API TestRegistry {
	public:
		static TestRegistry& getInstance();
		void runAll();
		void runGroup(const std::string& group);
		void addTest(const Test& test);
		Test& getThisTest();
		void TestLogOnOff(bool on);
	private:
		SimpleLogger log;
		int testNum = 0;
		bool isLogOn = true;
		void printTestInf(const Test& test);
		~TestRegistry();
		TestRegistry();
		int complited;
		std::vector<Test> tests;
	};

	

	template <typename Func,typename T>
	void runDataTest(Func func, std::vector<T> data) {
		for (auto& value : data) {
			func(value);
		}
	}
}
