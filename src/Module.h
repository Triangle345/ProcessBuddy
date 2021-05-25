#include <string>

class Module{
public:
    Module(
        std::string startAddr, 
        std::string endAddr,
        long long offset,
        std::string user,
        long long node,
        std::string path){

        };

    Module(std::string line)
    {
        char startend[50] ={0};
        char perms[50] ={0};
        char offset[50]={0};
        char user[50] = {0};
        char node[50] = {0};
        char path[50] = {0};
        sscanf(line.c_str(),"%s %s %s %s %s %s\n", startend,perms,offset, user, node, path);
        std::string startEndStr(startend);
        int pos = startEndStr.find("-");
        if (pos != std::string::npos)
        {
            this->startAddr = std::string(startEndStr.begin(), startEndStr.begin() + pos);
            this->endAddr = std::string(startEndStr.begin()+1 + pos, startEndStr.end());

        }
        this->perms = perms;
        this->offset = atoll(offset);
        this->user = user;
        this->node = atoll(node);
        this->path = path;
    }

    std::string startAddr = "";
    std::string endAddr = "";
    std::string perms = "";
    long long offset = 0;
    std::string user = "";
    long long node = 0;
    std::string path = "";
    
    static std::string getHeader()
    {
        // TODO: create constant for the buf size
        char buf[300] = {0};
        sprintf(buf, "Start End Permissions Offset User Node Path");
        return buf;
    }

    std::string toString(){
        char buf[600] = {0};
        sprintf(buf, "%s %s %s %lld %s %lld %s", 
        this->startAddr.c_str(), 
        this->endAddr.c_str(), 
        this->perms.c_str(), 
        this->offset, 
        this->user.c_str(), 
        this->node, 
        this->path.c_str());

        return buf;
    }
};