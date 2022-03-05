#include <iostream>
#include <string>

template <typename TLoggerImpl>
concept LoggerLike = requires(TLoggerImpl log) {
    log.LogDebug(std::string_view{});
    log.LogInfo(std::string_view{});
    log.LogError(std::string_view{});
};

template <LoggerLike TLoggerImpl>
struct Logger : TLoggerImpl {};

struct CustomLogger {
    void LogDebug(std::string_view message) const {
        std::cout << "[Debug] " << message << '\n';
    }
    void LogInfo(std::string_view message) const {
        std::cout << "[Info] " << message << '\n';
    }
    void LogError(std::string_view message) const {
        std::cout << "[Error] " << message << '\n';
    }
};

struct TestLogger {
    void LogDebug(std::string_view) const {}
    void LogInfo(std::string_view) const {}
    void LogError(std::string_view) const {}
};

template <LoggerLike TLogger>
void LogToAll(TLogger& logger, std::string_view message) {
    logger.LogDebug(message);
    logger.LogInfo(message);
    logger.LogError(message);
}

struct CustomLoggerImpl {
    void LogDebug(std::string_view message) const {
        std::cout << "[Debug] " << message << '\n';
    }
    void LogInfo(std::string_view message) const {
        std::cout << "[Info] " << message << '\n';
    }
    void LogError(std::string_view message) const {
        std::cout << "[Error] " << message << '\n';
    }
};

template <LoggerLike TLoggerImpl>
struct TaggedLogger : TLoggerImpl {
    TaggedLogger() = default;
    TaggedLogger(const std::string& tag) : m_tag(tag) {}

    void LogDebugTagged(const std::string& message) {
        const std::string& tagged = "[" + m_tag + "] " + message;
        static_cast<TLoggerImpl*>(this)->LogDebug(tagged);
        // there are no arguments to ‘LogDebug’ that depend on a template parameter, so a declaration of ‘LogDebug’ must be available
        // LogDebug(tagged); // does not compile
    }
    void SetTag(const std::string& t) { m_tag = t; }
    // ...
private:
    std::string m_tag;
};

using TaggedCustomLogger = TaggedLogger<CustomLoggerImpl>;

int main() {
    CustomLogger custom_logger;
    LogToAll(custom_logger, "Hello World");
    TestLogger test_logger;
    LogToAll(test_logger, "Hello World");

    TaggedCustomLogger logger;
    logger.SetTag("MyTag");
    logger.LogDebugTagged("Hello World");

    return 0;
}
