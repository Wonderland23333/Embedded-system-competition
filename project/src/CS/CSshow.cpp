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
    tableWidget.setColumnCount(3);  // 设置3列
    tableWidget.setHorizontalHeaderLabels({"编号", "姓名", "年龄"});  // 设置表头

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
    query.exec("SELECT * FROM shower_dataitem");
    int row = 0;
    while (query.next()) {
        // 插入一行数据
        tableWidget.insertRow(row);

        // 设置数据项
        QTableWidgetItem *item1 = new QTableWidgetItem(query.value(0).toString());
        QTableWidgetItem *item2 = new QTableWidgetItem(query.value(1).toString());
        QTableWidgetItem *item3 = new QTableWidgetItem(query.value(2).toString());
        tableWidget.setItem(row, 0, item1);
        tableWidget.setItem(row, 1, item2);
        tableWidget.setItem(row, 2, item3);

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
