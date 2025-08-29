
#include <string_view>
#include <vector>
namespace Utilities::Constants {
constexpr std::string_view c_LoggerModuleName{"LOGGER"};
constexpr int c_DefaultServerPort{8080};

const std::vector<std::string_view> c_ModuleNames{
    c_LoggerModuleName,
};

}  // namespace Utilities::Constants
