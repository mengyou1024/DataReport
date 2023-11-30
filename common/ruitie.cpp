#include "ruitie.h"

#include <QDebug>
#include <QFile>
#include <QPoint>

Q_LOGGING_CATEGORY(TAG_Ruitie, "Ruitie");

bool Ruitie::loadFile(const QString &fileName, RecData &data) {
    bool ret = false;
    data.setValid(false);
    QFile file(fileName);
    if (!file.exists()) {
        qCritical() << "file:" << fileName << "is not exsist";
        return false;
    }
    file.open(QFile::ReadOnly);
    if (!file.isOpen()) {
        qCritical() << "file:" << fileName << "open failed";
        return false;
    }
    uint32_t dwKey = 0, dwLastKey = 0, dwRead = 0;
    file.read((char *)&dwKey, sizeof(dwKey));
    if (dwKey == FK_NEWFILE) {
        while (file.read((char *)&dwKey, sizeof(dwKey))) {
            file.read((char *)&dwRead, sizeof(dwRead));
            switch (dwKey) {
                case FK_DATA_SYSFILE: {
                    // 系统参数表
                    if (dwRead == sizeof(System)) {
                        file.read((char *)&data.paramSystem, dwRead);
                    }
                    break;
                }
                case FK_DATA_CHFILE: {
                    // 通道参数
                    if (dwRead == HD_CHANNEL_NUM) {
                        for (uint32_t i = 0; i < HD_CHANNEL_NUM; i++) {
                            file.read((char *)&data.paramChannel[i], sizeof(Channel));
                        }
                    }

                    break;
                }
                case FK_DATA_SPECIFILE: {
                    // 车轮参数
                    if (dwRead == sizeof(WHEEL_PAPA)) {
                        file.read((char *)&data.wheelParam, dwRead);
                    }
                    break;
                }
                case FK_DATA_PLCFILE: {
                    // PLC参数
                    if (dwRead == sizeof(PLC_SCAN_PAPA)) {
                        file.read((char *)&data.plcScanParam, dwRead);
                    }
                    break;
                }
                case FK_DATA_USERFILE: {
                    // 用户数据
                    if (dwRead == sizeof(DB_USER_DATA)) {
                        file.read((char *)&data.dbUserData, dwRead);
                    }
                    break;
                }
                case FK_DATA_STDDETECTFILE: {
                    // 探伤数据
                    if (dwRead == sizeof(DETECTION_PARAM2995_200) * HD_CHANNEL_NUM) {
                        file.read((char *)&data.paramDetection, dwRead);
                    }
                    break;
                }

                case FK_DATA_SCANFILE: {
                    // 扫差数据
                    for (DWORD i = 0; i < dwRead; i++) {
                        RECORD_DATA temRecord = {};
                        file.read((char *)temRecord.pGatePos, sizeof(__int32) * 2 * 10);
                        file.read((char *)temRecord.pGateAmp, sizeof(__int32) * 2 * 10);
                        file.read((char *)temRecord.iAscanSize, sizeof(__int64) * 10);

                        for (int i = 0; i < HD_CHANNEL_NUM; i++) {
                            temRecord.pAscan[i].resize(temRecord.iAscanSize[i]);
                            file.read((char *)temRecord.pAscan[i].data(), temRecord.iAscanSize[i]);
                        }
                        data.m_pRecord.push_back(temRecord);
                    }

                    // 清空缺陷数据
                    // for (int i = 0; i < HD_CHANNEL_NUM; i++) {
                    //     QVector<DB_DEFECT_DATA *>::iterator it = data.m_pDefect[i].begin();
                    //     while (it != data.m_pDefect[i].end()) {
                    //         delete *it;
                    //         *it = NULL;
                    //         it++;
                    //     }
                    //     data.m_pDefect[i].clear();
                    // }
                    break;
                }

                case FK_DATA_DEFECTFILE: {
                    // 缺陷数据
                    for (DWORD ch = 0; ch < dwRead; ch++) {
                        DWORD dwVerWrite = 0;
                        file.read((char *)&dwVerWrite, sizeof(dwVerWrite));
                        int size = dwVerWrite / sizeof(DB_DEFECT_DATA);
                        for (int i = 0; i < size; i++) {
                            auto temDEFECT_DATA = std::make_shared<DB_DEFECT_DATA>();
                            file.read((char *)temDEFECT_DATA.get(), sizeof(DB_DEFECT_DATA)); // 数据读取
                            data.m_pDefect[ch].push_back(temDEFECT_DATA);
                        }
                    }
                    break;
                }

                case FK_DATA_PLCSPEED: {
                    if (dwRead == sizeof(PLC_SPeed_PAPA)) {
                        file.read((char *)&data.plcSpeedParam, dwRead);
                    }
                    break;
                }
                case FK_DATA_GROOVESHIELD: {
                    if (dwRead == sizeof(DETECTION_GROOVE_SHIELD) * HD_CHANNEL_NUM) {
                        file.read((char *)&data.detectionGroovShield, dwRead);
                    }
                    break;
                }

                case FK_DATA_END: {
                    // 数据结束
                    ret = true;
                    data.setValid(true);
                    goto __exit;
                }
                default: {
                    qCritical(TAG_Ruitie) << QString("read RecData error, lastKey:0x%1 Key: 0x%2").arg(QString::number(dwLastKey, 16), QString::number(dwKey, 16));
                    goto __exit;
                }
            }
            dwLastKey = dwKey;
        }
    } else {
        qCritical(TAG_Ruitie) << QString("read RecData error, dwKey:0x%1").arg(QString::number(dwKey, 16));
    }
__exit:
    file.close();
    return ret;
}
