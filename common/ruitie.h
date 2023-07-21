#pragma once

#include "xlsxdocument.h"
#include <QDate>
#include <QFile>
#include <QLoggingCategory>
#include <QProcess>
#include <memory>
#include <type_traits>

Q_DECLARE_LOGGING_CATEGORY(TAG_Ruitie);

namespace Ruitie {

    using std::make_shared;
    using std::shared_ptr;
    using namespace QXlsx;

    /**
     * @brief loadFile 加载文件
     * @param fileName 文件名
     * @return 数据指针
     */
    template <typename... T>
    bool loadFile(QString fileName, T *...ptr) {
        QFile file(fileName);
        if (!file.exists()) {
            qCritical(TAG_Ruitie) << "file:" << fileName << "is not exist";
            return false;
        }
        file.open(QFile::ReadOnly);
        if (!file.isOpen()) {
            qCritical(TAG_Ruitie) << "file:" << fileName << "open failed";
            return false;
        }
        qDebug(TAG_Ruitie) << "file:" << fileName << file.size() << "bytes";

        (file.read(reinterpret_cast<char *>(ptr), sizeof(T)), ...);

        file.close();
        return true;
    }
    template <typename... T>
    bool loadFile(QString fileName, shared_ptr<T>... ptr) {
        qDebug(TAG_Ruitie) << "loadFile(filename, ...)<shared_ptr>";
        return loadFile(fileName, (ptr.get(), ...));
    }

    /**
     * @brief saveFile 保存文件
     * @param fileName 文件名
     * @param ptr 数据指针
     * @return bool
     */
    template <typename... T>
    bool saveFile(QString fileName, T *...ptr) {
        if (fileName.isEmpty()) {
            qCritical(TAG_Ruitie) << "fileName is empty";
            return false;
        }
        QFile file(fileName);
        file.open(QFile::WriteOnly);
        if (!file.isOpen()) {
            qCritical(TAG_Ruitie) << fileName << "open failed";
            return false;
        }
        file.resize(0);
        (file.write(reinterpret_cast<char *>(ptr), sizeof(T)), ...);
        file.close();
        return true;
    }

    template <typename... T>
    bool saveFile(QString fileName, shared_ptr<T>... ptr) {
        qDebug(TAG_Ruitie) << "saveFile(filename, ...)<shared_ptr>";
        return saveFile(fileName, (ptr.get(), ...));
    }

    class DiscoveryRecordMsg;
    class InspectionRecordMsg;
    class QuarterlyRecordMsg;
    class DailyRecordMsg;
    template <>
    bool saveFile(QString fileName, DiscoveryRecordMsg *ptr);
    template <>
    bool saveFile(QString fileName, InspectionRecordMsg *ptr);
    template <>
    bool saveFile(QString fileName, QuarterlyRecordMsg *ptr);
    template <>
    bool saveFile(QString fileName, DailyRecordMsg *ptr);

    // 探伤记录模型
    class InspectionRecordModel : public QObject {
        Q_OBJECT
    public:
        QString wheelType;   ///< 车轮型号
        QString heatSerial;  ///< 炉号
        QString wheelSerial; ///< 车轮编号
        qreal   defectsNum;  ///< 缺陷数目
        qreal   radial;      ///< 径向
        qreal   axial;       ///< 轴向
        qreal   waveHeight;  ///< 波高
        qreal   dBDiff;      ///< dB差
        QString result;      ///< 探伤结论
        qreal   sensitivity; ///< 灵敏度
    };

    // 缺陷记录模型
    class defectRecordModel : public QObject {
        Q_OBJECT
    public:
        qreal axial;      ///< 轴向
        qreal radial;     ///< 径向
        qreal gain;       ///< 增益
        qreal waveHeight; ///< 波高
        qreal dBDiff;     ///< dB差
    };

    class performanceModel : public QObject {
        Q_OBJECT
    public:
    };

    /**
     * @brief Excel代理
     */
    class ExcelRender : public QXlsx::Document {
    public:
        void initColumnWidth(std::initializer_list<double> cWidht) {
            for (int i = 0; i < cWidht.size(); i++) {
                setColumnWidth(i + 1, *(cWidht.begin() + i) * 1.25);
            }
            columnCount = cWidht.size();
        }

        void initColumnWidth(double width, int count) {
            for (int i = 0; i < count; i++) {
                setColumnWidth(i + 1, width * 1.25);
            }
            columnCount = count;
        }

        template <typename T>
        void ExcelFill(T label, int columnSpan = 1, int rowSpan = 1) {
            Format format;
            format.setHorizontalAlignment(Format::AlignHCenter);
            format.setVerticalAlignment(Format::AlignVCenter);
            format.setBorderStyle(Format::BorderThin);
            write({cursor.x(), cursor.y()}, label, format);
            if (columnSpan != 1 || rowSpan != 1) {
                mergeCells(CellRange({cursor.x(), cursor.y()}, {cursor.x() + rowSpan - 1, cursor.y() + columnSpan - 1}), format);
            }
            cursor.setY(cursor.y() + columnSpan);
        }

        template <typename contentT>
        void ExcelFill(QString label, contentT content, int ctx_columnSpan = 1, int ctx_rowSpan = 1, int lb_columnSpan = 1,
                       int lb_rowSpan = 1) {
            ExcelFill<QString>(label, lb_columnSpan, lb_rowSpan);
            ExcelFill<contentT>(content, ctx_columnSpan, ctx_rowSpan);
        }

        void ExcelTableHead(QString label) {
            setRowHeight(cursor.x(), 36.75);
            ExcelFill<QString>(label, columnCount);
        }

        void ExcelNextRow(int bias = 0, double height = 14.25) {
            cursor.setX(cursor.x() + 1);
            cursor.setY(bias + 1);
            setRowHeight(cursor.x(), height * 1.25);
        }

    private:
        QPoint cursor      = {1, 1};
        int    columnCount = 0;
    };
    class ExcelRunner : public QObject {
        Q_OBJECT
    public:
        Q_INVOKABLE void openExcel(const QString &progma) {
            QProcess process(nullptr);
            QObject::connect(&process, &QProcess::readyReadStandardOutput, [&process]() { qInfo() << process.readAllStandardOutput(); });
            QObject::connect(&process, &QProcess::readyReadStandardError, [&process]() { qCritical() << process.readAllStandardError(); });
            process.start("cmd", QStringList() << "/c"
                                               << "start excel" << progma);
            process.waitForStarted();
            process.waitForFinished();
        }
    };

} // namespace Ruitie
