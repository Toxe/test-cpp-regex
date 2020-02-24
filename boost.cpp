#include <boost/regex.hpp>
#include <iostream>
#include <string>
#include <vector>

int main()
{
    const std::vector<std::string> lines{
        "[05821BE4 | 2019-05-13 12:28:56] (13036) http://test.site/projects/cmd.php [co_project.view] RQST START",
        "[05821BE4 | 2019-05-13 12:28:56] (13036) http://test.site/projects/cmd.php [co_project.view] RQST END   [normal]   402 ms",
        "[05671FA1 | 2019-04-17 14:08:03] (62931) http://test.site/projects/cmd.php [co_project.view] RQST START",
        "[05671FA1 | 2019-04-17 14:08:03] (62931) http://test.site/projects/cmd.php [co_project.view] RQST END   [normal]   237 ms",
        "[05822AE4 | 2019-06-17 06:59:03] (15828) http://test.site/cmd.php [co_project.dialog_doc_details] RQST START",
        "[05822AE4 | 2019-06-17 06:59:03] (15828) http://test.site/cmd.php [co_project.dialog_doc_details] RQST END   [normal]   318 ms",
        "[00180D0F | 2009-09-15 09:34:47] (127.0.0.1:39170, 879) /cmd.php [co_search.browse] RQST START",
        "[00180D0F | 2009-09-15 09:34:48] (127.0.0.1:39170, 879) /cmd.php [co_search.browse] RQST END   [normal]   799 ms",
        "[00180D0D | 2009-09-15 09:34:19] (127.0.0.1:39169, 23727) /browse/ RQST START",
        "[00180D0D | 2009-09-15 09:34:19] (127.0.0.1:39169, 23727) /browse/ RQST END   [normal]    35 ms",
        "[001F86EA | 2009-11-02 16:05:50] (127.0.0.1:1789, 10994) /cmd.php [co_doc.details] RQST START",
        "[001F86EA | 2009-11-02 16:05:50] (127.0.0.1:1789, 10994) /cmd.php [co_doc.details] RQST END   [normal]    84 ms"};

    const boost::regex re{R"(\[([^ ]{8}) \| ([^\]]{19})\] \((?:[^,]+, )?\d+\) [^ ]+ \[([^\]]+)\] RQST END   \[[^\]]+\] *(\d+) ms)"};
    boost::smatch m;

    for (auto line : lines)
        if (boost::regex_match(line, m, re))
            if (m.size() > 1)
                for (int i = 1; i < static_cast<int>(m.size()); ++i)
                    std::cout << "MATCH " << i << ": \"" << m[i] << "\"" << std::endl;
}
