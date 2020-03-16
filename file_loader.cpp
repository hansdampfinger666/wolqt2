#include "file_loader.h"


FileLoader::FileLoader(QObject *parent) : QObject(parent)
{
}


FileLoader::~FileLoader()
{
    std::cout << "am i being called?" << std::endl;
}


void
FileLoader::LoadConfig(const std::string &program_path)
{
    Benchmarker::start_timer();

    QString program_path_qt;
    config_path_ = program_path + config_file_name_linux_;
    QFileInfo file_info(program_path_qt.fromStdString(config_path_));

    std::string data;
    std::vector<bool> found_params;
    found_params.assign(device_params_.size(), false);
    FoundParam found_param;

    if(file_info.exists() && file_info.isFile())
    {
        std::ifstream data_stream(config_path_);

        while(getline(data_stream, data))
        {
            if(data.empty() || data.find_first_of("\"") == 0 || data.find("//") == 0)
                continue;

            data = Trim(data);
            found_param = ReadValue(data, found_params, device_params_);

            if(!found_param.value.empty() && found_param.param_index == 0)
            {
                found_params.assign(device_params_.size(), false);
                CreateNewValueSet(found_param);;
            }
            else if(!found_param.value.empty())
            {
                found_params.at(found_param.param_index) = true;
                AddValueToSet(found_param);
            }
        }
    }
    Benchmarker::end_timer(__PRETTY_FUNCTION__);
}


FileLoader::FoundParam
FileLoader::ReadValue(const std::string &data,
                      const std::vector<bool> &found_params,
                      const std::vector<std::string> &params)
{
    unsigned int param_index = 0;
    for(auto param : params)
    {
        if(found_params.at(param_index))
        {
            param_index++;
            continue;
        }
        if(data.substr(0, param.size()) == param)
            return { data.substr(param.size()), param_index };
        param_index++;
    }
    return { "", 0 };
}


void
FileLoader::CreateNewValueSet(const FoundParam &param)
{
    devices_.names.push_back(param.value);
    devices_.macs.push_back("");
    devices_.ostypes.push_back("");
    devices_.ssh_pubkey_paths.push_back("");
    devices_.ssh_users.push_back("");
    devices_.ips.push_back("");
}


void
FileLoader::AddValueToSet(const FoundParam &param)
{
    unsigned int set = devices_.names.size() - 1;

    switch(param.param_index)
    {
    case 1:
        devices_.macs.at(set) = param.value;
        break;
    case 2:
        devices_.ostypes.at(set) = param.value;
        break;
    case 3:
        devices_.ssh_pubkey_paths.at(set) = param.value;
        break;
    case 4:
        devices_.ssh_users.at(set) = param.value;
        break;
    case 5:
        devices_.ips.at(set) = param.value;
        break;
    }
}


std::string
FileLoader::Trim(std::string &str)
{
    return str = str.substr(str.find_first_not_of(" "), str.find_last_not_of(" ") + 1);
}
