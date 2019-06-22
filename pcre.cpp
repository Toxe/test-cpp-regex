#define OVECCOUNT 30

#include <iostream>
#include <pcre.h>
#include <string>
#include <vector>

int main()
{
    const std::vector<std::string> lines{
        "[05821BE4 | 2019-05-13 12:28:56] (13036) http://contiview.site/projects/cmd.php [co_project.view] RQST START",
        "[05821BE4 | 2019-05-13 12:28:56] (13036) http://contiview.site/projects/cmd.php [co_project.view] RQST END   [normal]   402 ms",
        "[05671FA1 | 2019-04-17 14:08:03] (62931) http://contiview.site/projects/cmd.php [co_project.view] RQST START",
        "[05671FA1 | 2019-04-17 14:08:03] (62931) http://contiview.site/projects/cmd.php [co_project.view] RQST END   [normal]   237 ms",
        "[05822AE4 | 2019-06-17 06:59:03] (15828) http://contiview.site/cmd.php [co_project.dialog_doc_details] RQST START",
        "[05822AE4 | 2019-06-17 06:59:03] (15828) http://contiview.site/cmd.php [co_project.dialog_doc_details] RQST END   [normal]   318 ms",
        "[00180D0F | 2009-09-15 09:34:47] (193.98.108.243:39170, 879) /cmd.php [co_search.browse] RQST START",
        "[00180D0F | 2009-09-15 09:34:48] (193.98.108.243:39170, 879) /cmd.php [co_search.browse] RQST END   [normal]   799 ms",
        "[00180D0D | 2009-09-15 09:34:19] (193.98.108.243:39169, 23727) /browse/ RQST START",
        "[00180D0D | 2009-09-15 09:34:19] (193.98.108.243:39169, 23727) /browse/ RQST END   [normal]    35 ms",
        "[001F86EA | 2009-11-02 16:05:50] (193.98.108.243:1789, 10994) /cmd.php [co_doc.details] RQST START",
        "[001F86EA | 2009-11-02 16:05:50] (193.98.108.243:1789, 10994) /cmd.php [co_doc.details] RQST END   [normal]    84 ms"};

    const char* error;
    int erroroffset;

    const char* pattern = R"(\[([^ ]{8}) \| ([^\]]{19})\] \((?:[^,]+, )?\d+\) [^ ]+ \[([^\]]+)\] RQST END   \[[^\]]+\] *(\d+) ms)";
    pcre* re = pcre_compile(pattern, 0, &error, &erroroffset, nullptr);

    if (!re) {
        std::cout << "PCRE compilation failed at offset " << erroroffset << ": " << error << std::endl;
        return 1;
    }

    pcre_extra* sd = pcre_study(re, 0, &error);

    if (!sd && error) {
        std::cout << "PCRE study error: " << error << std::endl;
        return 2;
    }

    for (auto line : lines) {
        int ovector[OVECCOUNT];
        const int rc = pcre_exec(re, sd, line.c_str(), line.size(), 0, 0, ovector, OVECCOUNT);

        if (rc > 1) {
            for (int i = 1; i < rc; ++i) {
                const char* substring_start = line.c_str() + ovector[2*i];
                const int substring_length = ovector[2*i + 1] - ovector[2*i];
                const std::string_view s{substring_start, (std::size_t) substring_length};
                std::cout << "MATCH " << i << ": \"" << s << "\"" << std::endl;
            }
        }
    }

    pcre_free_study(sd);
    pcre_free(re);
}
