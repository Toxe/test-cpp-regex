#include <hs/hs.h>
#include <iostream>
#include <string>
#include <vector>

static int eventHandler(unsigned int id, unsigned long long from, unsigned long long to, unsigned int flags, void* ctx)
{
    std::string* s = (std::string*) ctx;
    std::cout << "MATCH from " << from << " to " << to << " in: " << *s << std::endl;
    return 0;
}

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

    const char* re{R"(\[([^ ]{8}) \| ([^\]]{19})\] \((?:[^,]+, )?\d+\) [^ ]+ \[([^\]]+)\] RQST END   \[[^\]]+\] *(\d+) ms)"};

    hs_database_t* database;
    hs_compile_error_t* compile_err;

    if (hs_compile(re, HS_FLAG_DOTALL | HS_FLAG_SINGLEMATCH, HS_MODE_BLOCK, nullptr, &database, &compile_err) != HS_SUCCESS) {
        std::cout << "Hyperscan unable to compile pattern: " << compile_err->message << std::endl;
        hs_free_compile_error(compile_err);
        return 1;
    }

    hs_scratch_t* scratch = nullptr;

    if (hs_alloc_scratch(database, &scratch) != HS_SUCCESS) {
        std::cout << "Hyperscan unable to allocate scratch space." << std::endl;
        hs_free_database(database);
        return 2;
    }

    for (auto line : lines) {
        if (hs_scan(database, line.c_str(), line.length(), 0, scratch, eventHandler, &line) != HS_SUCCESS) {
            std::cout << "Hyperscan scan error." << std::endl;
            hs_free_scratch(scratch);
            hs_free_database(database);
            return 3;
        }
    }

    hs_free_scratch(scratch);
    hs_free_database(database);
}
