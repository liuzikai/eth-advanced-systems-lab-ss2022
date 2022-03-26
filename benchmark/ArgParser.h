#ifndef ASL_TEAM02_ARGPARSER_H
#define ASL_TEAM02_ARGPARSER_H

#include <string>
#include <vector>
#include <optional>

// This is taken from here:
// https://stackoverflow.com/questions/865668/parsing-command-line-arguments-in-c
class ArgParser{
public:
    ArgParser (int &argc, char **argv);
    /// @author iain
    std::optional<const std::string_view> getCmdOption(const std::string &option) const;
    std::optional<size_t> getCmdOptionAsInt(const std::string &option) const;
    /// @author iain
    bool cmdOptionExists(const std::string &option) const;
private:
    std::vector <std::string> tokens;
};


#endif //ASL_TEAM02_ARGPARSER_H


