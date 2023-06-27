
#include <QApplication>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsEllipseItem>
#include <QGraphicsLineItem>
#include <QGraphicsTextItem>
#include <QPen>
#include <QTimer>
#include <QGraphicsProxyWidget>
#include <QPushButton>
#include <QDialog>
#include <QVBoxLayout>
#include <QObject>
#include <QSpinBox>
#include <QLabel>
#include "graph.h"
#include "randGraph.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QPushButton *button_rand = new QPushButton(QString("生成随机图"));
    QPushButton *button_start= new QPushButton(QString("开始最短路演示"));
    button_rand->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    button_rand->setFixedWidth(400);
    button_rand->setFixedHeight(100);
    button_start->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    button_start->setFixedWidth(400);
    button_start->setFixedHeight(100);

    QDialog window;
    QVBoxLayout *layout = new QVBoxLayout(&window);

    QHBoxLayout *layout_button = new QHBoxLayout(&window);

    layout_button->addWidget(button_rand);
    layout_button->setAlignment(button_rand, Qt::AlignBottom | Qt::AlignLeft);

    QGraphicsScene *empty = new QGraphicsScene;
    QGraphicsView *v_empty = new QGraphicsView(empty);
    layout->addWidget(v_empty);
    layout->addLayout(layout_button);

    QLabel *label1 = new QLabel("最短路演示起点：");
    QSpinBox *spinBox1 = new QSpinBox;

    QLabel *label2 = new QLabel("最短路演示终点：");
    QSpinBox *spinBox2 = new QSpinBox;

    label1->setMinimumSize(100, 0);
    label1->setMaximumSize(16777215, 50);
    spinBox1->setMinimumSize(100, 0);
    spinBox1->setMaximumSize(16777215, 50);
    spinBox2->setMinimumSize(100, 0);
    spinBox2->setMaximumSize(16777215, 50);
    label1->setMinimumSize(100, 0);
    label1->setMaximumSize(16777215, 50);

    window.resize(1400,850);
    window.setWindowTitle("最短路求取动态演示");
    window.setWindowIcon(QIcon(":/icon.ico"));
    window.show();
    graph now;
    static QGraphicsView *lastView = nullptr;
    QObject::connect(button_rand, &QPushButton::clicked, [&]() {

        randGraph();
        now.build_graph();
        QGraphicsScene *graphScene=now.print_graph();
        QGraphicsView *view = new QGraphicsView(graphScene);
        if (lastView != nullptr) {
            lastView->hide();
            layout->removeWidget(lastView);
        }else
        {
            layout->removeWidget(v_empty);
            layout_button->addWidget(label1);
            layout_button->addWidget(spinBox1);
            layout_button->addWidget(label2);
            layout_button->addWidget(spinBox2);
            layout_button->addWidget(button_start);
        }
        spinBox1->setRange(1, now.getN());
        spinBox1->setValue(1);
        spinBox2->setRange(1, now.getN());
        spinBox2->setValue(now.getN());
        lastView = view;
        layout->addWidget(view);
        layout->removeItem(layout_button);
        layout_button->setAlignment(button_start, Qt::AlignBottom | Qt::AlignRight);
        layout->addLayout(layout_button);

        QObject::connect(button_start,&QPushButton::clicked, [&]() {
            now.refresh();
            button_start->setEnabled(false);
            button_rand->setEnabled(false);
            int st_point=spinBox1->value();
            int en_point=spinBox2->value();
            now.show_paths(now.shortest_path(st_point,en_point));
        });

        QObject::connect(&now, &graph::finished, [=]() {
            button_start->setEnabled(true);
            button_rand->setEnabled(true);
        });
        window.update();
        window.show();
    });

    return a.exec();
}
