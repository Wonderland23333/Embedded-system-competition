#include <QApplication>
#include <QMainWindow>
#include <QTableWidget>
#include <QVBoxLayout>
#include <QtSql>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    // 创建主窗口
    QMainWindow mainWindow;
    mainWindow.setWindowTitle("数据列表");

    // 创建表格
    QTableWidget tableWidget;
    tableWidget.setColumnCount(5);  // 设置5列
    tableWidget.setHorizontalHeaderLabels({"a倾角", "b倾角", "b辐射" ,"直接辐射", "散射辐射"});  // 设置表头

    // 连接数据库
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("/root/loongarch/project/sensor.db");

    // 打开数据库
    if (!db.open()) {
        qDebug() << "无法连接数据库";
        return 1;
    }

    // 查询数据
    QSqlQuery query;
    query.exec("SELECT * FROM shower_sensordata");
    int row = 0;
    while (query.next()) {
        // 插入一行数据
        tableWidget.insertRow(row);

        // 设置数据项
        QTableWidgetItem *a_angle = new QTableWidgetItem(query.value(0).toString());//太阳光与水平面夹角
        QTableWidgetItem *R_radiate = new QTableWidgetItem(query.value(1).toString());//倾角为b的斜面上的辐射
        QTableWidgetItem *S_radiate = new QTableWidgetItem(query.value(2).toString());//水平面上直接辐射
        QTableWidgetItem *D_radiate = new QTableWidgetItem(query.value(3).toString());//水平面上散射辐射
        QTableWidgetItem *b_angle = new QTableWidgetItem(query.value(4).toString());//太阳能电池方阵倾角
        tableWidget.setItem(row, 0, a_angle);
        tableWidget.setItem(row, 1, b_angle);
        tableWidget.setItem(row, 2, R_radiate);
        tableWidget.setItem(row, 3, S_radiate);
        tableWidget.setItem(row, 4, D_radiate);


        row++;
    }

    // 关闭数据库
    db.close();

    // 将表格添加到主窗口
    QVBoxLayout *layout = new QVBoxLayout();
    layout->addWidget(&tableWidget);
    QWidget *centralWidget = new QWidget();
    centralWidget->setLayout(layout);
    mainWindow.setCentralWidget(centralWidget);

    // 显示主窗口
    mainWindow.show();

    return app.exec();
}
