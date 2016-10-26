/****************************************************************************
**
** Copyright (C) 2016 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of Qt Creator.
**
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3 as published by the Free Software
** Foundation with exceptions as appearing in the file LICENSE.GPL3-EXCEPT
** included in the packaging of this file. Please review the following
** information to ensure the GNU General Public License requirements will
** be met: https://www.gnu.org/licenses/gpl-3.0.html.
**
****************************************************************************/

#include "openeditorsview.h"
#include "editormanager.h"
#include "ieditor.h"
#include "documentmodel.h"

#include <coreplugin/coreicons.h>
#include <coreplugin/actionmanager/actionmanager.h>
#include <coreplugin/actionmanager/command.h>
#include <coreplugin/modemanager.h>
#include <coreplugin/cycameramanager.h>
#include <coreplugin/cyframeparsermanager.h>

#include <CYCore/cycamerafactory.h>
#include <CYCore/cyframeparserfactory.h>

#include <QApplication>
#include <QMenu>
#include <QStandardItemModel>
#include <QMessageBox>

using namespace CYCore;
using namespace Core;
using namespace Core::Internal;


typedef enum tagCUSTOMROLE
{
    ROLE_NODE_LEVEL = Qt::UserRole + 1, // 指示树状结构的等级
    ROLE_NODE_ID,
    ROLE_TEST_CONNECT_STATUS
}CUSTOMROLE;
////
// OpenEditorsWidget
////

OpenEditorsWidget::OpenEditorsWidget()
{
    setWindowTitle(tr("Open Documents"));
    setWindowIcon(Icons::DIR.icon());
    setDragEnabled(true);
    setDragDropMode(QAbstractItemView::DragOnly);
    //setEditable();
    QStandardItemModel * modle = new QStandardItemModel();
    m_model = new ProxyModel(this);
    m_model->setSourceModel(DocumentModel::model());
    // 添加表头
    modle->setHorizontalHeaderLabels(QStringList() << QStringLiteral("相机列表")/* << QStringLiteral("链接状态")*/);
    // 添加行和列以及节点
    QStandardItem * item = 0;
#if 0
    if (ModeManager::currentMode() == Id("Edit"))
    {
        item = new QStandardItem(tr("AD0"));
        modle->appendRow(item);
        item->setEditable(false);
        item->setData(0, ROLE_NODE_LEVEL);
        // 添加另一行和列及节点
        item = new QStandardItem(tr("AB3"));
        modle->appendRow(item);
        item->setEditable(false);
        item->setData(0, ROLE_NODE_LEVEL);
    }
    else
    {
        item = new QStandardItem(tr("vendor1"));
        modle->appendRow(item);
        item->setEditable(false);
        item->setData(0, ROLE_NODE_LEVEL);
        // 添加另一行和列及节点
        item = new QStandardItem(tr("vendor12"));
        modle->appendRow(item);
        item->setEditable(false);
        item->setData(0, ROLE_NODE_LEVEL);
    }
#else
    CYCameraManager::CYCameraFactoryList factorys = CYCameraManager::getCameraFactorys(ModeManager::currentMode());
    foreach(CYCameraFactory * factory, factorys)
    {
        item = new QStandardItem(factory->displayName());
        item->setEditable(false);
        item->setData(0, ROLE_NODE_LEVEL);
        item->setData(factory->id().toSetting(), ROLE_NODE_ID);
        modle->appendRow(item);
    }
#endif
    setModel(/*m_model*/modle);

    setContextMenuPolicy(Qt::CustomContextMenu);

    /*connect(EditorManager::instance(), &EditorManager::currentEditorChanged,
            this, &OpenEditorsWidget::updateCurrentItem);
    connect(this, &OpenDocumentsTreeView::activated,
            this, &OpenEditorsWidget::handleActivated);
    connect(this, &OpenDocumentsTreeView::closeActivated,
            this, &OpenEditorsWidget::closeDocument);
    */
    connect(this, &QTreeView::customContextMenuRequested,
        this, &OpenEditorsWidget::contextMenuRequested);
    connect(this, &QTreeView::activated, this, [this](QModelIndex index) {
        if (index.isValid())
        {
            QVariant i_mode = index.data(ROLE_NODE_LEVEL);
            if (i_mode == 0)
            {
                // 选择本列的第一个显示
            }
            else if (i_mode == 1)
            {
                QVariant ls_name = index.data();
                EditorManager::activeSubEditorView(ls_name.toString().toStdString().c_str());
            }
        }
    });
}

OpenEditorsWidget::~OpenEditorsWidget()
{
}

void OpenEditorsWidget::updateCurrentItem(IEditor *editor)
{
    IDocument *document = editor ? editor->document() : 0;
    QModelIndex index = m_model->index(DocumentModel::indexOfDocument(document), 0);
    if (!index.isValid()) {
        clearSelection();
        return;
    }
    setCurrentIndex(index);
    selectionModel()->select(currentIndex(),
        QItemSelectionModel::ClearAndSelect | QItemSelectionModel::Rows);
    scrollTo(currentIndex());
}

void OpenEditorsWidget::handleActivated(const QModelIndex &index)
{
    if (index.column() == 0) {
        activateEditor(index);
    }
    else if (index.column() == 1) { // the funky close button
        closeDocument(index);

        // work around a bug in itemviews where the delegate wouldn't get the QStyle::State_MouseOver
        QPoint cursorPos = QCursor::pos();
        QWidget *vp = viewport();
        QMouseEvent e(QEvent::MouseMove, vp->mapFromGlobal(cursorPos), cursorPos, Qt::NoButton, 0, 0);
        QCoreApplication::sendEvent(vp, &e);
    }
}

void OpenEditorsWidget::activateEditor(const QModelIndex &index)
{
    selectionModel()->select(index, QItemSelectionModel::ClearAndSelect | QItemSelectionModel::Rows);
    EditorManager::activateEditorForEntry(
        DocumentModel::entryAtRow(m_model->mapToSource(index).row()));
}

void OpenEditorsWidget::closeDocument(const QModelIndex &index)
{
    EditorManager::closeDocument(
        DocumentModel::entryAtRow(m_model->mapToSource(index).row()));
    // work around selection changes
    updateCurrentItem(EditorManager::currentEditor());
}
// 右键菜单
void OpenEditorsWidget::contextMenuRequested(QPoint pos)
{
    QMenu contextMenu;
    QModelIndex editorIndex = indexAt(pos);
    //DocumentModel::Entry *entry = DocumentModel::entryAtRow(
    //            m_model->mapToSource(editorIndex).row());
    //EditorManager::addSaveAndCloseEditorActions(&contextMenu, entry);
    //contextMenu.addSeparator();
    //EditorManager::addNativeDirAndOpenWithActions(&contextMenu, entry);
    if (!editorIndex.isValid())
    {
        return;
    }
    // 这是一级设备驱动的右键菜单
    if (editorIndex.data(ROLE_NODE_LEVEL) == 0)
    {
        contextMenu.addAction("Serach", this, [this, editorIndex]() {

            QVariant vale = editorIndex.data();
            Id factoryId = Id::fromSetting(editorIndex.data(ROLE_NODE_ID));
            // 搜索
            // 
            int cameracount = CYCore::CYCameraManager::SerachCamera(factoryId);
            // 可以使用QTimer::singleShot();
            QMessageBox::information(0, tr("Serach!"), tr("Serach a more Camera Success!"));
            QList<Id> cameraList;
            for (int i = 0;i < cameracount;i++)
            {
                cameraList << Id::fromSetting(i);
            }
            //
            QStandardItem * itemChild = 0;
            QStandardItem * itemParent = 0;
            QStandardItemModel * modle = qobject_cast<QStandardItemModel *>((QAbstractItemModel*)editorIndex.model());
            itemParent = modle->itemFromIndex(editorIndex);
            int rowCount = itemParent->rowCount();
            itemParent->removeRows(0, rowCount);
            foreach(Id id, cameraList)
            {
                if (itemParent)
                {
                    QString listName = vale.toString() + "." + id.toString();
                    Id cameraId = Id::fromSetting(factoryId.toString() + "." + id.toString());
                    itemChild = new QStandardItem(listName);
                    itemChild->setEditable(false);
                    itemChild->setData(1, ROLE_NODE_LEVEL);
                    itemChild->setData(0, ROLE_TEST_CONNECT_STATUS);
                    itemChild->setData(cameraId.toSetting(), ROLE_NODE_ID);
                    itemParent->appendRow(itemChild);
                    EditorManager::createSubEditorView(Id::fromSetting(listName));
                }
            }
        });
    }
    else
    {
        Id cameraid = Id::fromSetting(editorIndex.data(ROLE_NODE_ID));
        // 这里判断设备的连接状态
        // 暂时通过
        //if (editorIndex.data(ROLE_TEST_CONNECT_STATUS) == 0)
        if (!CYCameraManager::isConnect(cameraid))
        {
            //CYCameraManager::isConnect(cameraid);
            contextMenu.addAction("Connect", this, [this, editorIndex,cameraid]() {
                CYCameraManager::connectCamera(cameraid);
                QMessageBox::information(
                    0, tr("Connect!"), tr("Connect Camera Success!"));
                // 通知editManager创建窗口
            });
        }
        else
        {
            contextMenu.addAction("disConnect", this, [this, editorIndex,cameraid]() {
                CYCameraManager::disconnectCamera(cameraid);
                QMessageBox::information(
                    0, tr("Connect!"), tr("disConnect Camera Success!"));
                // 通知editManager销毁窗口
            });
            // 增加开始采集的右键菜单
            if (!CYCameraManager::isCapture(cameraid))
            {
                contextMenu.addAction("StartCapture", this, [this, editorIndex, cameraid]() {
                    CYCameraManager::startCapture(cameraid);
                    QMessageBox::information(
                        0, tr("StartCapture!"), tr("StartCapture Camera Success!"));
                    // 通知editManager创建窗口
                });
            }
            else
            {
                contextMenu.addAction("StopCapture", this, [this, editorIndex, cameraid]() {
                    CYCameraManager::stopCapture(cameraid);
                    QMessageBox::information(
                        0, tr("StopCapture!"), tr("StopCapture Camera Success!"));
                    // 通知editManager创建窗口
                });
            }
            // 增加图像处理的右键菜单
            QList<CYFrameParserFactory*> factorys = CYFrameParserManager::getProcessorFactorys();
            QMenu * contextParserMenu = 0;
            QMenu * contextAlyzerMenu = 0;
            if (!factorys.isEmpty())
            {
                contextMenu.addSeparator();
            }
            foreach(CYFrameParserFactory*factory, factorys)
            {
                switch (factory->type())
                {
                case CYFrameParserFactory::CYFRAMEPARSER_ANALYZER:
                {
                    if (!contextAlyzerMenu)
                    {
                        contextAlyzerMenu = contextMenu.addMenu(tr("Camera Alyzer"));
                    }
                    contextAlyzerMenu->addAction(factory->displayName(), this, [this, editorIndex, factory]() {
                        QVariant cams = editorIndex.data();
                        //EditorManager::createProcessEditorView(cams.toString().toStdString().c_str(), factory->id());
                        // 向系统添加完用于展示的窗体后，这里要向指定的id的相机添加一个处理器,这样就可以由系统进行调度处理了
                        // CYCameraManager::app
                        // 分析类插件需要向OutputPanlManager进行委托处理
                    });
                }
                break;
                case CYFrameParserFactory::CYFRAMEPARSER_PROCESSOR:
                {
                    if (!contextParserMenu)
                    {
                        contextParserMenu = contextMenu.addMenu(tr("Camera Parser"));
                    }
                    contextParserMenu->addAction(factory->displayName(), this, [this, editorIndex, factory, cameraid]() {
                        QVariant cams = editorIndex.data();
                        EditorManager::createProcessEditorView(Id::fromSetting(cams), factory->id());
                        // 激活本窗体

                        // 向系统添加完用于展示的窗体后，这里要向指定的id的相机添加一个处理器,这样就可以由系统进行调度处理了
                        CYCameraManager::appendFrameParser(cameraid, factory->createFrameParser());
                        // CYCameraManager::app
                    });
                }
                break;
                default:
                    break;
                }
            }
        }
    }
    contextMenu.exec(/*mapToGlobal(pos)*/QCursor::pos());
}

///
// OpenEditorsViewFactory
///

OpenEditorsViewFactory::OpenEditorsViewFactory()
{
    setId("Open Documents");
    setDisplayName(OpenEditorsWidget::tr("Open Documents"));
    setActivationSequence(QKeySequence(UseMacShortcuts ? tr("Meta+O") : tr("Alt+O")));
    setPriority(200);
}

NavigationView OpenEditorsViewFactory::createWidget()
{
    return NavigationView(new OpenEditorsWidget());
}

ProxyModel::ProxyModel(QObject *parent) : QAbstractProxyModel(parent)
{
}

QModelIndex ProxyModel::mapFromSource(const QModelIndex &sourceIndex) const
{
    // root
    if (!sourceIndex.isValid())
        return QModelIndex();
    // hide the <no document>
    int row = sourceIndex.row() - 1;
    if (row < 0)
        return QModelIndex();
    return createIndex(row, sourceIndex.column());
}

QModelIndex ProxyModel::mapToSource(const QModelIndex &proxyIndex) const
{
    if (!proxyIndex.isValid())
        return QModelIndex();
    // handle missing <no document>
    return sourceModel()->index(proxyIndex.row() + 1, proxyIndex.column());
}

QModelIndex ProxyModel::index(int row, int column, const QModelIndex &parent) const
{
    if (parent.isValid() || row < 0 || row >= sourceModel()->rowCount(mapToSource(parent)) - 1
        || column < 0 || column > 1)
        return QModelIndex();
    return createIndex(row, column);
}

QModelIndex ProxyModel::parent(const QModelIndex &child) const
{
    Q_UNUSED(child)
        return QModelIndex();
}

int ProxyModel::rowCount(const QModelIndex &parent) const
{
    if (!parent.isValid())
        return sourceModel()->rowCount(mapToSource(parent)) - 1;
    return 0;
}

int ProxyModel::columnCount(const QModelIndex &parent) const
{
    return sourceModel()->columnCount(mapToSource(parent));
}

void ProxyModel::setSourceModel(QAbstractItemModel *sm)
{
    QAbstractItemModel *previousModel = sourceModel();
    if (previousModel) {
        disconnect(previousModel, &QAbstractItemModel::dataChanged,
            this, &ProxyModel::sourceDataChanged);
        disconnect(previousModel, &QAbstractItemModel::rowsInserted,
            this, &ProxyModel::sourceRowsInserted);
        disconnect(previousModel, &QAbstractItemModel::rowsRemoved,
            this, &ProxyModel::sourceRowsRemoved);
        disconnect(previousModel, &QAbstractItemModel::rowsAboutToBeInserted,
            this, &ProxyModel::sourceRowsAboutToBeInserted);
        disconnect(previousModel, &QAbstractItemModel::rowsAboutToBeRemoved,
            this, &ProxyModel::sourceRowsAboutToBeRemoved);
    }
    QAbstractProxyModel::setSourceModel(sm);
    if (sm) {
        connect(sm, &QAbstractItemModel::dataChanged,
            this, &ProxyModel::sourceDataChanged);
        connect(sm, &QAbstractItemModel::rowsInserted,
            this, &ProxyModel::sourceRowsInserted);
        connect(sm, &QAbstractItemModel::rowsRemoved,
            this, &ProxyModel::sourceRowsRemoved);
        connect(sm, &QAbstractItemModel::rowsAboutToBeInserted,
            this, &ProxyModel::sourceRowsAboutToBeInserted);
        connect(sm, &QAbstractItemModel::rowsAboutToBeRemoved,
            this, &ProxyModel::sourceRowsAboutToBeRemoved);
    }
}

QModelIndex ProxyModel::sibling(int row, int column, const QModelIndex &idx) const
{
    return QAbstractItemModel::sibling(row, column, idx);
}

Qt::DropActions ProxyModel::supportedDragActions() const
{
    return sourceModel()->supportedDragActions();
}

void ProxyModel::sourceDataChanged(const QModelIndex &topLeft, const QModelIndex &bottomRight)
{
    QModelIndex topLeftIndex = mapFromSource(topLeft);
    if (!topLeftIndex.isValid())
        topLeftIndex = index(0, topLeft.column());
    QModelIndex bottomRightIndex = mapFromSource(bottomRight);
    if (!bottomRightIndex.isValid())
        bottomRightIndex = index(0, bottomRight.column());
    emit dataChanged(topLeftIndex, bottomRightIndex);
}

void ProxyModel::sourceRowsRemoved(const QModelIndex &parent, int start, int end)
{
    Q_UNUSED(parent)
        Q_UNUSED(start)
        Q_UNUSED(end)
        endRemoveRows();
}

void ProxyModel::sourceRowsInserted(const QModelIndex &parent, int start, int end)
{
    Q_UNUSED(parent)
        Q_UNUSED(start)
        Q_UNUSED(end)
        endInsertRows();
}

void ProxyModel::sourceRowsAboutToBeRemoved(const QModelIndex &parent, int start, int end)
{
    int realStart = parent.isValid() || start == 0 ? start : start - 1;
    int realEnd = parent.isValid() || end == 0 ? end : end - 1;
    beginRemoveRows(parent, realStart, realEnd);
}

void ProxyModel::sourceRowsAboutToBeInserted(const QModelIndex &parent, int start, int end)
{
    int realStart = parent.isValid() || start == 0 ? start : start - 1;
    int realEnd = parent.isValid() || end == 0 ? end : end - 1;
    beginInsertRows(parent, realStart, realEnd);
}
