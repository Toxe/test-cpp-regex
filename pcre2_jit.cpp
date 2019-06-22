#define PCRE2_CODE_UNIT_WIDTH 8

#include <iostream>
#include <pcre2.h>
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

    int errorcode;
    PCRE2_SIZE erroroffset;

    const PCRE2_SPTR pattern = (PCRE2_SPTR) R"(\[([^ ]{8}) \| ([^\]]{19})\] \((?:[^,]+, )?\d+\) [^ ]+ \[([^\]]+)\] RQST END   \[[^\]]+\] *(\d+) ms)";
    pcre2_code* re = pcre2_compile(pattern, PCRE2_ZERO_TERMINATED, 0, &errorcode, &erroroffset, nullptr);

    if (!re) {
        PCRE2_UCHAR buffer[256];
        pcre2_get_error_message(errorcode, buffer, sizeof(buffer));
        std::cout << "PCRE2 compilation failed at offset " << erroroffset << ": " << buffer << std::endl;
        return 1;
    }

    if ((errorcode = pcre2_jit_compile(re, PCRE2_JIT_COMPLETE)) < 0) {
        PCRE2_UCHAR buffer[256];
        pcre2_get_error_message(errorcode, buffer, sizeof(buffer));
        std::cout << "PCRE2 JIT compile error " << erroroffset << ": " << buffer << std::endl;
        return 2;
    }

    pcre2_match_context* mcontext = pcre2_match_context_create(nullptr);

    if (!mcontext) {
        std::cout << "PCRE2 unable to create match context" << std::endl;
        return 3;
    }

    pcre2_jit_stack* jit_stack = pcre2_jit_stack_create(32*1024, 512*1024, nullptr);

    if (!jit_stack) {
        std::cout << "PCRE2 unable to create JIT stack" << std::endl;
        return 4;
    }

    pcre2_jit_stack_assign(mcontext, nullptr, jit_stack);

    pcre2_match_data* match_data = pcre2_match_data_create_from_pattern(re, nullptr);

    if (!match_data) {
        std::cout << "PCRE2 unable to create match data" << std::endl;
        return 5;
    }

    for (auto line : lines) {
        const PCRE2_SPTR subject = (PCRE2_SPTR) line.c_str();
        const int rc = pcre2_jit_match(re, subject, line.size(), 0, 0, match_data, mcontext);

        if (rc > 1) {
            const PCRE2_SIZE* ovector = pcre2_get_ovector_pointer(match_data);

            for (int i = 1; i < rc; ++i) {
                const PCRE2_SPTR substring_start = subject + ovector[2*i];
                const int substring_length = ovector[2*i + 1] - ovector[2*i];
                const std::string_view s{(const char*) substring_start, (std::size_t) substring_length};
                std::cout << "MATCH " << i << ": \"" << s << "\"" << std::endl;
            }
        }
    }

    pcre2_match_data_free(match_data);
    pcre2_jit_stack_free(jit_stack);
    pcre2_match_context_free(mcontext);
    pcre2_code_free(re);
}
