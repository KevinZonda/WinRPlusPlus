#ifndef CONFIG_H
#define CONFIG_H

#include <array>

constexpr int MAX_HISTORY_COUNT = 10;

#ifndef Q_OS_WINDOWS
constexpr std::array<const char *const, 4> TERMINALS = {"zsh", "fish", "bash", "sh"};
#endif

#endif // CONFIG_H
