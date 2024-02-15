#pragma once

#include <QDebug>
#include <QFile>
#include <QString>
#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <vector>
#include <windows.h>

namespace Ruitie {
    constexpr int HD_CHANNEL_NUM = 10;                     ///< 硬件通道数
    constexpr int DAC_HOLE_NUM   = 10;                     ///< DAC 采集孔的个数

    constexpr uint32_t FK_NEWFILE            = 0x55545041; ///< 开始位
    constexpr uint32_t FK_DATA_SYSFILE       = 0x55545042; ///< 系统参数位 System
    constexpr uint32_t FK_DATA_CHFILE        = 0x55545043; ///< 通道参数位  Channel*10
    constexpr uint32_t FK_DATA_SPECIFILE     = 0x55545044; ///< 工件参数/车轮参数
    constexpr uint32_t FK_DATA_PLCFILE       = 0x55545045; ///< PLC下发的参数
    constexpr uint32_t FK_DATA_USERFILE      = 0x55545046; ///< 当前探伤工信息
    constexpr uint32_t FK_DATA_STDDETECTFILE = 0x55545047; ///< 标准探伤数据
    constexpr uint32_t FK_DATA_SCANFILE      = 0x55545048; ///< 扫差数据
    constexpr uint32_t FK_DATA_DEFECTFILE    = 0x55545049; ///< 缺陷数据
    constexpr uint32_t FK_DATA_END           = 0x55545050; ///< 数据结束

    constexpr uint32_t FK_DATA_PLCSPEED     = 0x55545051;  ///< PLC速度参数
    constexpr uint32_t FK_DATA_GROOVESHIELD = 0x55545052;  ///< 刻槽屏蔽参数

    struct System {
        int32_t Frequency;     ///< 重复频率
        int32_t Voltage;       ///< 电压
        int32_t PulseWidth;    ///< 脉冲宽度
        int32_t TxFlag;        ///< 发射标志
        int32_t RxFlag;        ///< 接收标志
        int32_t ChMode;        ///< 通道工作模式
        int32_t ScanIncrement; ///< 扫查增量
        int32_t ResetCoder;    ///< 编码器清零
        int32_t LEDStatus;     ///< LED灯
        int32_t WorkType;      ///< 工作模式
        int32_t ControlTime;
        int32_t AxleTime;
    };

    struct Channel {
        uint32_t m_ChannelParamLen;  ///< 通道大小
        float    m_fRange;           ///< 声程
        int32_t  m_iVelocity;        ///< 声速
        float    m_fDelay;           ///< 延时
        float    m_fOffset;          ///< 零点
        int32_t  m_iSampleFactor;    ///< 采样因子
        float    m_fGain;            ///< 增益
        int32_t  m_iFilter;          ///< 滤波
        int32_t  m_iDemodu;          ///< 检波方式
        int32_t  m_iPhaseReverse;    ///< 相位反转

        int32_t m_pGateAlarmType[2]; ///< 波门报警类型
        float   m_pGatePos[2];       ///< 波门门位
        float   m_pGateWidth[2];     ///< 波门宽度
        float   m_pGateHeight[2];    ///< 波门高度

        int32_t m_iGateBType;        ///< 波门2类型
    };

    // 车轮参数
    struct WHEEL_PAPA {
        TCHAR szWheelUseModel[20];    // 适用车型  动车组
        TCHAR szWheelType[20];        // 车轮类型  轮对
        TCHAR szWheelName[20];        // 车轮名称
        TCHAR szWheelModel[20];       // 车轮型号
        TCHAR szWheelMaterial[20];    // 车轮材质  铸刚
        TCHAR szWheelNumber[20];      // 车轮编号
        TCHAR szHeatNumber[20];       // 炉号
        TCHAR szDetectionStd[20];     // 探伤标准 TB/T2995-200
        TCHAR szDetectionContent[50]; // 探伤内容   缺陷小于Ф2mm平地孔当量，内侧面检测时低波衰减不低于4dB
        TCHAR szDetectionArea[20];    // 探伤区域  踏面 内侧面
        TCHAR szDetectionFact[50];    // 检测单位
        TCHAR szDeviceName[50];       // 设备名称
        // 车轮尺寸
        FLOAT fWheelHub;               // 轮毂厚度  1
        FLOAT fWheelRim;               // 轮辋厚度 2
        FLOAT fWheelInnerSideOffset;   // 轮毂内侧面与轮辋内侧面高度差（-n~+n） 3
        FLOAT fWheelHubInnerDiameter;  // 车轮内径 4
        FLOAT fWheelHubOuterDiameter;  // 轮毂外径 5
        FLOAT fWheelRimlInnerDiameter; // 轮辋内径 6
        FLOAT fWheelRimOuterDiameter;  // 轮辋外径 7
        // 其他待定参数
        FLOAT fTreadWidth;        // 踏面探头中心间距（3组 轴向方向的间距，用于计算探头分段覆盖）
        FLOAT fSideWidth;         // 侧面探头中心间距 （2组 径向方向的间距，用于计算探头分段覆盖）

        FLOAT fWheelFlangeHeight; // 轮缘厚度 （计算 踏面检测范围 轮辋厚度减去轮缘厚度）
        FLOAT fWheelParam4;
    };

    struct PLC_SCAN_PAPA {
        // 扫差参数相关 自动控制相关 数值
        // Y方向以 轮轴中心为原点 向右位正 向左为负
        // Y方向以探头归0位置为原点，向下为正。

        // 侧面相关
        float nSideYPos;   ///< 侧面探头压合无须上下扫差移动

        float nSideXStart; ///< 侧面探头的X起始点
        float nSideXEnd;   ///< 侧面探头的X结束点
        float nSideXStep;  ///< 侧面探头的步进值
        float nSideParam1; ///< 待定参数
        float nSideParam2; ///< 待定参数
        // 踏面相关
        float nTreadXPos;   ///< 踏面探头检测位置，无须左右移动

        float nTreadYStart; ///< 踏面探头的Y起始点
        float nTreadYEnd;   ///< 踏面探头的Y结束点
        float nTreadYStep;  ///< 踏面探头的步进值
        float nTreadParam1; ///< 待定参数
        float nTreadParam2; ///< 待定参数
    };

    // 人员信息
    struct DB_USER_DATA {
        long  lSerial;       // 自动编号
        TCHAR strName[21];   // 检测人员
        TCHAR strNumber[21]; // 工号
        TCHAR strPwd[21];    // 密码
        long  lLevel;        // 权限
        TCHAR strRemark[41]; // 备注
    };

    // 数据记录
    struct RECORD_DATA {
        int32_t       pGatePos[2];                ///< 波门位置
        int32_t       pGateAmp[2];                ///< 波门波幅
        int64_t       iAscanSize[HD_CHANNEL_NUM]; ///< pAscan 大小
        QVector<BYTE> pAscan[HD_CHANNEL_NUM];     ///< data
    };

    struct DETECTION_PARAM2995_200 {
        FLOAT fScanGain;                    // 探伤灵敏度
        FLOAT fScanCompensate;              // 探伤补偿灵敏度

        FLOAT fScanTrUTGain;                // 透声灵敏度
        FLOAT fScanTrUTCompensate;          // 透声补偿

        FLOAT fAperture;                    // 孔径
        int   nUseHole;                     // 采样孔数

        FLOAT fDAC_HoleDepth[DAC_HOLE_NUM]; // Dac 的孔深 位置  10 - 30 -50 -100mm
        FLOAT fDAC_HolebAmp[DAC_HOLE_NUM];  // Dac的 孔的波幅 百分比   0-1

        FLOAT fDAC_BaseGain;                // 制作Dac 的基准增益 母线增益 第一孔的增益
        FLOAT fDAC_LineRD;                  // 记录线 与母线的dB
        FLOAT fDAC_LineRL;                  // 判废线
        FLOAT nDACGatePos[2];               // 波门门位 百分比  0-1 取值
        FLOAT nDACWidth[2];                 // 波门宽度 百分比  0-1 取值

        //	 其他待定参数
        float fGateAAmpPos; // 动态DAC 记录制作DAC 第一个点的A门最高波  后面车轮踏面检测需要动态DAC
        float fAMaxAmp_0;   // 距A门最高波 作为检测起始位置
    };

    // 缺陷信息 一个检测数据有多条缺陷信息
    struct DB_DEFECT_DATA {
        int   nIndex;              // 缺陷索引号 当前通道内
        int   nCircleIndex;        // 缺陷在最高波时所在圈数
        int   nScanIndex;          // 缺陷在最高波时扫差数据的索引
        int   nCH;                 // 发现缺陷的通道
        TCHAR szProbeMake[10];     // 探测架名称 探头标识 A1 B1之类
        TCHAR szDetectionArea[20]; // 缺陷部位 踏面 /侧面
        float fProbleYPos;         // 发现缺陷时探头位置Y        nCH 0-5 为踏面位置     6-9为侧面位置
        float fProbleXPos;         // 发现缺陷时探头位置X

        float nRadialDistance;     // 径向距离  //踏面探头为 缺陷深度       侧面探头为探头位置
        float nAxialDepth;         // 轴向深度    //踏面探头为 探头位置      侧面探头为缺陷深度
        float nDefectAngle;        // 缺陷角度
        int   nWaveHeight;         // 波高 0-255

        float nDBOffset;           // dB差
        float nSensitivity;        // 探伤灵敏度
        float nTranslucency;       // 透声灵敏度
        int   bDefectType;         // 0 缺陷 1. 透声不良

        //	 其他待定参数
        int   nParam1; // 首次发现缺陷的索引
        int   nParam2; // 缺陷结束的索引
        TCHAR szParam3[20];
        TCHAR szParam4[20];
    };

    struct DB_QUARTER_DATA {
        char  m_dtTime[10];             // 测试时间
        TCHAR m_szPerson[20];           // 测试人员姓名或工号
        float m_nHorLinearity[10];      // 水平线性  以下五个为五大性能，季度是否合格通过5个性能值范围判断
        float m_nVerLinearity[10];      // 垂直线行
        float m_nDistinguishValuel[10]; // 分辨力
        float m_nDynamicRange[10];      // 动态范围
        float m_nSensitivityMargin[10]; // 动态范围灵敏度余量

        TCHAR szProbeType[10];          // 探头类型
        INT32 nProbeFrequency;          // 探头频率
        int   nPreProbeNum;             // 探头编号
        TCHAR szProbeMake[10];          // 探头标识 A1 B1之类

        TCHAR m_szStdTestBlock[20];     // 标准试块型号

        //	 其他待定参数
        int   nParam1;
        int   nParam2;
        TCHAR szParam3[20];
        TCHAR szParam4[20];
    };

    struct DETECTION_GROOVE_SHIELD // 刻槽屏蔽参数
    {
        float fGrooveRadialPos;    // 刻槽径向位置
        float fGrooveRadialWidth;  // 刻槽径向宽度
        float fGrooveAxialPos;     // 超声屏蔽位置
        float fGrooveAxialWidth;   // 超声屏蔽宽度

        int nParam1;               // 预留参数位置
        int nParam2;
        int nParam3;
        int nParam4;
    };

    struct PLC_SPeed_PAPA {
        /**
    扫差参数速度参数
    */

        // 左侧面相关  手动/自动
        float fSideXSpeed1;
        float fSideXSpeed2;
        float fSideYSpeed1;
        float fSideYSpeed2;

        // 右踏面
        float fTreadXSpeed1;
        float fTreadXSpeed2;
        float fTreadYSpeed1;
        float fTreadYSpeed2;

        // 转盘
        float fRotateSpeed1;
    };

    class RecData {
    public:
        System                                   paramSystem;
        Channel                                  paramChannel[HD_CHANNEL_NUM];
        WHEEL_PAPA                               wheelParam;
        PLC_SCAN_PAPA                            plcScanParam;
        DB_USER_DATA                             dbUserData;
        DETECTION_PARAM2995_200                  paramDetection[HD_CHANNEL_NUM];
        PLC_SPeed_PAPA                           plcSpeedParam;
        DETECTION_GROOVE_SHIELD                  detectionGroovShield[HD_CHANNEL_NUM];
        QVector<RECORD_DATA>                     m_pRecord;
        QVector<std::shared_ptr<DB_DEFECT_DATA>> m_pDefect[HD_CHANNEL_NUM];

        inline bool isValid(void) const noexcept {
            return m_Valid;
        }

        inline void setValid(bool newValid) noexcept {
            m_Valid = newValid;
        }

    private:
        bool m_Valid = false;
    };

} // namespace Ruitie
