#ifndef FILE_LOADER_H
#define FILE_LOADER_H

#include <QObject>
#include <QString>
#include <QFileInfo>

#include <string>
#include <vector>
#include <array>

#include <fstream>
#include <algorithm>

#include <iostream>
#include <benchmark.h>


class FileLoader : public QObject
{
    Q_OBJECT

public:
    typedef struct{
        std::string value;
        unsigned int param_index;
    } FoundParam;

    struct Devices{
        std::vector<std::string> names;
        std::vector<std::string> macs;
        std::vector<std::string> ostypes;
        std::vector<std::string> ssh_pubkey_paths;
        std::vector<std::string> ssh_users;
        std::vector<std::string> ips;
    } devices_ {};

    explicit FileLoader(QObject *parent = nullptr);
    ~FileLoader();
    void LoadConfig(const std::string &program_path);

    const std::string * GetDeviceName(unsigned int &device_index) { if(device_index > devices_.names.size() - 1) return nullptr;
                                                                    else return &devices_.names.at(device_index); };
    const std::string * GetDeviceMAC(unsigned int &device_index) { if(device_index > devices_.macs.size()- 1) return nullptr;
                                                                   else return &devices_.macs.at(device_index); };
    const std::string * GetDeviceOSType(unsigned int &device_index) { if(device_index > devices_.ostypes.size()- 1) return nullptr;
                                                                      else return &devices_.ostypes.at(device_index); };
    const std::string * GetDeviceSSHPubkeyPath(unsigned int &device_index) { if(device_index > devices_.ssh_pubkey_paths.size()- 1) return nullptr;
                                                                             else return &devices_.ssh_pubkey_paths.at(device_index); };
    const std::string * GetDeviceSSHUser(unsigned int &device_index) { if(device_index > devices_.ssh_users.size()- 1) return nullptr;
                                                                       else return &devices_.ssh_users.at(device_index); };

signals:

private:
    const std::string config_file_name_linux_ = "config";
    const std::vector<std::string> program_params_ { "debug_mode = ", "ultra_violence = " };
    const std::vector<std::string> device_params_ { "machine = ", "mac = ",
                                                    "ostype = ", "ssh_pubkey_path = ",
                                                    "ssh_user = " };

    std::string config_path_;

    FoundParam ReadValue(const std::string &data,
                         const std::vector<bool> &found_params,
                         const std::vector<std::string> &params);
    void CreateNewValueSet(const FoundParam &param);
    void AddValueToSet(const FoundParam &param);
    std::string Trim(std::string &str);
};

#endif // FILE_LOADER_H
