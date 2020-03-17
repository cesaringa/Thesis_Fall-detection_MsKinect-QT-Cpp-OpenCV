/********************************************************************************
** Form generated from reading UI file 'Kinect2Images.ui'
**
** Created by: Qt User Interface Compiler version 5.7.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_KINECT2IMAGES_H
#define UI_KINECT2IMAGES_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QGraphicsView>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Kinect2Images
{
public:
    QGridLayout *gridLayout;
    QGridLayout *gridLayout_2;
    QLabel *label;
    QLineEdit *lineEdit_label;
    QLineEdit *le_path;
    QLabel *label_2;
    QPushButton *pb_path;
    QLabel *label_3;
    QSpacerItem *horizontalSpacer_4;
    QSpacerItem *horizontalSpacer_3;
    QLineEdit *lineEdit_slot;
    QLabel *label_4;
    QLineEdit *lineEdit_lightsource;
    QLabel *lb_status;
    QHBoxLayout *horizontalLayout_2;
    QSpacerItem *horizontalSpacer;
    QPushButton *pb_shot;
    QSpacerItem *horizontalSpacer_2;
    QGraphicsView *gv_depthImage;
    QGraphicsView *gv_colorImage;

    void setupUi(QWidget *Kinect2Images)
    {
        if (Kinect2Images->objectName().isEmpty())
            Kinect2Images->setObjectName(QStringLiteral("Kinect2Images"));
        Kinect2Images->resize(1190, 626);
        gridLayout = new QGridLayout(Kinect2Images);
        gridLayout->setSpacing(6);
        gridLayout->setContentsMargins(11, 11, 11, 11);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        gridLayout_2 = new QGridLayout();
        gridLayout_2->setSpacing(6);
        gridLayout_2->setObjectName(QStringLiteral("gridLayout_2"));
        label = new QLabel(Kinect2Images);
        label->setObjectName(QStringLiteral("label"));

        gridLayout_2->addWidget(label, 0, 1, 1, 1);

        lineEdit_label = new QLineEdit(Kinect2Images);
        lineEdit_label->setObjectName(QStringLiteral("lineEdit_label"));

        gridLayout_2->addWidget(lineEdit_label, 2, 2, 1, 1);

        le_path = new QLineEdit(Kinect2Images);
        le_path->setObjectName(QStringLiteral("le_path"));

        gridLayout_2->addWidget(le_path, 0, 2, 1, 1);

        label_2 = new QLabel(Kinect2Images);
        label_2->setObjectName(QStringLiteral("label_2"));

        gridLayout_2->addWidget(label_2, 2, 1, 1, 1);

        pb_path = new QPushButton(Kinect2Images);
        pb_path->setObjectName(QStringLiteral("pb_path"));
        pb_path->setMinimumSize(QSize(31, 23));
        pb_path->setMaximumSize(QSize(31, 23));

        gridLayout_2->addWidget(pb_path, 0, 3, 1, 1);

        label_3 = new QLabel(Kinect2Images);
        label_3->setObjectName(QStringLiteral("label_3"));

        gridLayout_2->addWidget(label_3, 3, 1, 1, 1);

        horizontalSpacer_4 = new QSpacerItem(900, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_2->addItem(horizontalSpacer_4, 0, 4, 1, 1);

        horizontalSpacer_3 = new QSpacerItem(5, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_2->addItem(horizontalSpacer_3, 0, 0, 1, 1);

        lineEdit_slot = new QLineEdit(Kinect2Images);
        lineEdit_slot->setObjectName(QStringLiteral("lineEdit_slot"));

        gridLayout_2->addWidget(lineEdit_slot, 3, 2, 1, 1);

        label_4 = new QLabel(Kinect2Images);
        label_4->setObjectName(QStringLiteral("label_4"));

        gridLayout_2->addWidget(label_4, 1, 1, 1, 1);

        lineEdit_lightsource = new QLineEdit(Kinect2Images);
        lineEdit_lightsource->setObjectName(QStringLiteral("lineEdit_lightsource"));

        gridLayout_2->addWidget(lineEdit_lightsource, 1, 2, 1, 1);


        gridLayout->addLayout(gridLayout_2, 1, 0, 1, 2);

        lb_status = new QLabel(Kinect2Images);
        lb_status->setObjectName(QStringLiteral("lb_status"));

        gridLayout->addWidget(lb_status, 4, 0, 1, 2);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setSpacing(6);
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        horizontalSpacer = new QSpacerItem(468, 48, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer);

        pb_shot = new QPushButton(Kinect2Images);
        pb_shot->setObjectName(QStringLiteral("pb_shot"));
        pb_shot->setMinimumSize(QSize(201, 51));
        pb_shot->setMaximumSize(QSize(201, 51));
        QFont font;
        font.setPointSize(17);
        pb_shot->setFont(font);

        horizontalLayout_2->addWidget(pb_shot);

        horizontalSpacer_2 = new QSpacerItem(478, 48, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer_2);


        gridLayout->addLayout(horizontalLayout_2, 3, 0, 1, 2);

        gv_depthImage = new QGraphicsView(Kinect2Images);
        gv_depthImage->setObjectName(QStringLiteral("gv_depthImage"));
        gv_depthImage->setMinimumSize(QSize(461, 421));

        gridLayout->addWidget(gv_depthImage, 0, 1, 1, 1);

        gv_colorImage = new QGraphicsView(Kinect2Images);
        gv_colorImage->setObjectName(QStringLiteral("gv_colorImage"));
        gv_colorImage->setMinimumSize(QSize(701, 421));

        gridLayout->addWidget(gv_colorImage, 0, 0, 1, 1);


        retranslateUi(Kinect2Images);

        QMetaObject::connectSlotsByName(Kinect2Images);
    } // setupUi

    void retranslateUi(QWidget *Kinect2Images)
    {
        Kinect2Images->setWindowTitle(QApplication::translate("Kinect2Images", "Kinect2OneShot", 0));
        label->setText(QApplication::translate("Kinect2Images", "path", 0));
        le_path->setText(QApplication::translate("Kinect2Images", ".", 0));
        label_2->setText(QApplication::translate("Kinect2Images", "label", 0));
        pb_path->setText(QApplication::translate("Kinect2Images", "...", 0));
        label_3->setText(QApplication::translate("Kinect2Images", "slot height", 0));
        label_4->setText(QApplication::translate("Kinect2Images", "light source", 0));
        lb_status->setText(QApplication::translate("Kinect2Images", "--", 0));
        pb_shot->setText(QApplication::translate("Kinect2Images", "Shot...", 0));
    } // retranslateUi

};

namespace Ui {
    class Kinect2Images: public Ui_Kinect2Images {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_KINECT2IMAGES_H
