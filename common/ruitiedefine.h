#pragma once

#include <cstdint>

namespace Ruitie {

    using INT32    = int32_t;
    using INT8     = int8_t;
    using TCHAR    = char;
    using UINT32   = uint32_t;
    using BOOL     = bool;
    using DATE     = double;
    using COLORREF = unsigned long;

    // 系统参数 包括板卡系统参数， 设备参数  ，等其他一次性参数
    struct SYS_PARA {
        // 超声参数
        INT32 nFrequency;     // 重复频率 转换成下发的工作周期
        INT32 nPulseWidth;    // 脉冲宽度
        INT32 nTxFlag;        // 发射标志
        INT32 nRxFlag;        // 接收标志
        INT32 nChMode;        // 通道工作模式
        INT32 nScanIncrement; // 扫查增量
        INT32 nResetCoder;    // 编码器清零
        INT32 nLEDStatus;     // LED灯
        INT32 nWorkType;      // 工作模式
        INT8  nControlTime;   // 控制信号消抖时间
        INT8  nAxleTime;      // 计轴信号消抖时间
        INT32 nVoltage;       // 电压
        INT32 nCurrCh;        // 当前通道,最多10
        INT32 nMaxGate;       // 最多门数,最多8
        INT32 nDenoise;       // 平均次数

        // 设备参数
        TCHAR szMadeFact[20];   // 设备制造单位
        TCHAR szMadeDate[20];   // 设备制造日期
        TCHAR szMadeModal[20];  // 设备型号
        TCHAR szMadeSerial[20]; // 设备编号
        TCHAR szUseOrg[20];     // 使用机构 检测单位
        TCHAR szUseXiu1[20];    // 大修日期
        TCHAR szUseXiu2[20];    // 中修日期
        TCHAR szUseXiu3[20];    // 小修日期
        TCHAR szUseXiu4[20];    // 定检日期
        // 其他待定参数

        int   nParam1;
        int   nParam2;
        TCHAR szParam3[20];
        TCHAR szParam4[20];
    };

    // 通道参数
    struct CH_PARA {
        TCHAR szTechName[20]; // 工艺名称
        // 超声通道参数
        UINT32 nDelay;        // 延时
        UINT32 nSampleDepth;  // 采样深度
        INT8   nSampleFactor; // 采样因子
        INT8   nFilter;       // 滤波
        INT8   nDemodu;       // 检波方式
        INT8   nPhaseReverse; // 相位反转？
        INT8   nActive[2];    // 波门激活
        INT32  iRealSize;     // 实际采样点数  有声程和采样因子计算出下发
        int    nZero;         // 零点
        int    nAngle;        // 角度
        int    nSpeed1;       // 液体声速
        int    nSpeed2;       // 工件声速
        ///////////////////////////////////////////////////////////////////////
        int nCh_logic;          // 逻辑通道号   逻辑通道号 软通道，一般情况和收发一直，特殊情况收发不一致
        int nCh_tr;             // 发射,最多256
        int nCh_re;             // 接受,最多256

        int  nGain;             // 总增益  *10保存  = 灵敏度+补偿
        int  nSensitivityGain;  // 灵敏度
        int  nCompensateGain;   // 补偿
        int  nSensitivityGain2; // 透声灵敏度
        int  nCompensateGain2;  // 透声补偿
        int  nGposi[5];
        int  nGwide[5];
        int  nGhigh[5];
        int  nGtype[5];     // 当前门类型0进波1失波
        int  nGalarm[5];    // 报警，－1不报警0全,1门A，2门B，3门C
        BOOL nGRelative2_1; // 第二个门的性质，TRUE表示其门位是相对于门一内最大值的位置，FALSE表示独立

        int nBSCANGtype;    // 报警类型 0进波1失波

        int nBSCANGGalarm;  // 报警波门选择 0波门A 1波门B  其他待定

        // DAC相关参数
        int nDacDist[10]; // //点的声程
        int nDacDb[10];   // 点的波幅
        int nDacNum;      // 点数

        // 探头相关
        TCHAR szProbeType[10]; // 探头类型
        INT32 nProbeFrequency; // 探头频率
        int   nPreProbe;       // 探头前沿
        TCHAR szProbeMake[10]; // 探头标识 A1 B1之类

        // 通道相关
        int   nCHNum;                   // 通道编号 1.23...
        TCHAR szCHName[20];             // 通道名称 A1-径向
        TCHAR szDetectionWheelArea[20]; // 检测的车轮部位
    };

    // 自动控制相关的参数 需要下发plc
    struct PLC_SCAN_PAPA {
        /**
        扫查参数相关 自动控制相关 数值
        Y方向以 轮轴中心为原点 向右位正 向左为负
        Y方向以探头归0位置为原点，向下为正。
        */

        // 侧面相关
        int nSideYPos;   // 侧面探头压合无须上下扫查移动
        int nSideXStart; // 侧面探头的X起始点
        int nSideXEnd;   // 侧面探头的X结束点
        int nSideXStep;  // 侧面探头的步进值
        int nSideParam1; // 待定参数
        int nSideParam2; // 待定参数
        // 踏面相关
        int nTreadXPos;   // 踏面探头检测位置，无须左右移动
        int nTreadYStart; // 踏面探头的Y起始点
        int nTreadYEnd;   // 踏面探头的Y结束点
        int nTreadYStep;  // 踏面探头的步进值
        int nTreadParam1; // 待定参数
        int nTreadParam2; // 待定参数
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
        TCHAR szDetectionFact[20];    // 检测单位

        // 车轮尺寸
        int nWheelHub;               // 轮毂厚度
        int nWheelRim;               // 轮辋厚度
        int nWheelInnerSideOffset;   // 轮毂内侧面与轮辋内侧面高度差（-n~+n）
        int nWheelInnerDiameter;     // 车轮内径
        int nWheelHubOuterDiameter;  // 轮毂外径
        int nWheelRimlInnerDiameter; // 轮辋内径
        int nWheelRimOuterDiameter;  // 轮辋外径
        // 其他待定参数
        int nWheelParam1;
        int nWheelParam2;
        int nWheelParam3;
        int nWheelParam4;
    };

    // 日常数据  无效特别处理，使用样轮径向扫查 列出扫查缺陷的db 误差值。

    // 季度信息
    struct DB_QUARTER_DATA {
        DATE   m_dtTime;                 // 测试时间
        TCHAR  m_szPerson[20];           // 测试人员姓名或工号
        UINT32 m_nHorLinearity[10];      // 水平线性  以下五个为五大性能，季度是否合格通过5个性能值范围判断
        UINT32 m_nVerLinearity[10];      // 垂直线行
        UINT32 m_nDistinguishValuel[10]; // 分辨力
        UINT32 m_nDynamicRange[10];      // 动态范围
        UINT32 m_nSensitivityMargin;     // 动态范围灵敏度余量

        TCHAR szProbeType[10];           // 探头类型
        INT32 nProbeFrequency;           // 探头频率
        int   nPreProbeNum;              // 探头编号
        TCHAR szProbeMake[10];           // 探头标识 A1 B1之类

        TCHAR m_szStdTestBlock[20];      // 标准试块型号

        //	 其他待定参数
        int   nParam1;
        int   nParam2;
        TCHAR szParam3[20];
        TCHAR szParam4[20];
    };

    // 扫查数据
    struct DB_SCAN_DATA {
        TCHAR szWheelModel[20];   // 车轮型号
        TCHAR szWheelNumber[20];  // 车轮编号
        DATE  m_dtTime;           // 测试时间

        int  nTotalDefectNum;     // 总缺陷个数
        int  nTotalCircleNum;     // 总圈数最高20圈
        long lEachCircleSize[20]; //  每圈的采样数据（0-360度时 所记录的帧数）最高20圈
        long lTotalScanSize;      // 扫查总数据大小 byte单位   （所有圈数的10个通道的采样数据）
    };

    // 缺陷信息 一个检测数据有多条缺陷信息
    struct DB_DEFECT_DATA {
        int   nIndex;              // 缺陷索引号
        int   nCircleIndex;        // 缺陷所在圈数
        int   nCH;                 // 发现缺陷的通道
        TCHAR szProbeMake[10];     // 探测架名称 探头标识 A1 B1之类
        TCHAR szDetectionArea[20]; // 缺陷部位 踏面 /侧面
        int   nDefectAngle;        // 缺陷角度
        int   nRadialDistance;     // 径向距离
        int   nAxialDepth;         // 轴向深度
        int   nWaveHeight;         // 波高

        int nDBOffset;             // dB差
        int nTranslucency;         // 透声
        int nSensitivity;          // 灵敏度

        //	 其他待定参数
        int   nParam1;
        int   nParam2;
        TCHAR szParam3[20];
        TCHAR szParam4[20];
    };

    // 人员信息
    struct DB_USER_DATA {
        long  lSerial;       // 自动编号
        TCHAR strName[20];   // 检测人员
        TCHAR strNumber[20]; // 工号
        TCHAR strPwd[10];    // 密码
        long  lLevel;        // 权限
        TCHAR strRemark[40]; // 备注
    };

    /*
    车轮检测说明
    1.一条工艺数据 =   SYS_PARA （系统参数）+ CH_PARA （通道参数 *10）+PLC_SCAN_PAPA（PLC参数）+ WHEEL_PAPA（车轮参数）
    2.季度数据   = SYS_PARA （系统参数）+  DB_QUARTER_DATA（季度数据） + DB_USER_DATA（检测人员信息）
    3.日常数据/扫查数据   =    一条工艺数据 + DB_USER_DATA（检测人员信息）+DB_SCAN_DATA（扫查数据）+
    动态的缺陷数据（nTotalDefectNum*DB_DEFECT_DATA）+动态的扫查数据（lTotalScanSize个扫查数据）

    //日常数据/扫查数据的区别为 日常数据 单独保存

    */

    // 原成像数据
    struct CURVE_INFO {
        int      x;     // 曲线区起始位置点
        int      y;
        int      cx;    // 曲线区水平点数
        int      cy;    // 曲线区垂直点数
        int      start; // 曲线在画图区之后的开始点
        int      dots;  // 曲线点数
        int      high;  // 门高度（只用于直方门）
        COLORREF color; // 曲线颜色
    };
} // namespace Ruitie
