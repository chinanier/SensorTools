/********************************************************************************
** Form generated from reading UI file 'pluginerroroverview.ui'
**
** Created by: Qt User Interface Compiler version 5.7.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_PLUGINERROROVERVIEW_H
#define UI_PLUGINERROROVERVIEW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialog>
#include <QtWidgets/QDialogButtonBox>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QVBoxLayout>

namespace ExtensionSystem {
namespace Internal {

class Ui_PluginErrorOverview
{
public:
    QVBoxLayout *verticalLayout;
    QLabel *label;
    QListWidget *pluginList;
    QLabel *label_2;
    QTextEdit *pluginError;
    QDialogButtonBox *buttonBox;

    void setupUi(QDialog *ExtensionSystem__Internal__PluginErrorOverview)
    {
        if (ExtensionSystem__Internal__PluginErrorOverview->objectName().isEmpty())
            ExtensionSystem__Internal__PluginErrorOverview->setObjectName(QStringLiteral("ExtensionSystem__Internal__PluginErrorOverview"));
        ExtensionSystem__Internal__PluginErrorOverview->resize(434, 361);
        verticalLayout = new QVBoxLayout(ExtensionSystem__Internal__PluginErrorOverview);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        label = new QLabel(ExtensionSystem__Internal__PluginErrorOverview);
        label->setObjectName(QStringLiteral("label"));
        label->setWordWrap(true);

        verticalLayout->addWidget(label);

        pluginList = new QListWidget(ExtensionSystem__Internal__PluginErrorOverview);
        pluginList->setObjectName(QStringLiteral("pluginList"));

        verticalLayout->addWidget(pluginList);

        label_2 = new QLabel(ExtensionSystem__Internal__PluginErrorOverview);
        label_2->setObjectName(QStringLiteral("label_2"));

        verticalLayout->addWidget(label_2);

        pluginError = new QTextEdit(ExtensionSystem__Internal__PluginErrorOverview);
        pluginError->setObjectName(QStringLiteral("pluginError"));
        pluginError->setReadOnly(true);

        verticalLayout->addWidget(pluginError);

        buttonBox = new QDialogButtonBox(ExtensionSystem__Internal__PluginErrorOverview);
        buttonBox->setObjectName(QStringLiteral("buttonBox"));
        buttonBox->setOrientation(Qt::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::NoButton);

        verticalLayout->addWidget(buttonBox);


        retranslateUi(ExtensionSystem__Internal__PluginErrorOverview);
        QObject::connect(buttonBox, SIGNAL(accepted()), ExtensionSystem__Internal__PluginErrorOverview, SLOT(accept()));
        QObject::connect(buttonBox, SIGNAL(rejected()), ExtensionSystem__Internal__PluginErrorOverview, SLOT(reject()));

        QMetaObject::connectSlotsByName(ExtensionSystem__Internal__PluginErrorOverview);
    } // setupUi

    void retranslateUi(QDialog *ExtensionSystem__Internal__PluginErrorOverview)
    {
        ExtensionSystem__Internal__PluginErrorOverview->setWindowTitle(QApplication::translate("ExtensionSystem::Internal::PluginErrorOverview", "Qt Creator - Plugin loader messages", 0));
        label->setText(QApplication::translate("ExtensionSystem::Internal::PluginErrorOverview", "The following plugins have errors and cannot be loaded:", 0));
        label_2->setText(QApplication::translate("ExtensionSystem::Internal::PluginErrorOverview", "Details:", 0));
    } // retranslateUi

};

} // namespace Internal
} // namespace ExtensionSystem

namespace ExtensionSystem {
namespace Internal {
namespace Ui {
    class PluginErrorOverview: public Ui_PluginErrorOverview {};
} // namespace Ui
} // namespace Internal
} // namespace ExtensionSystem

#endif // UI_PLUGINERROROVERVIEW_H
