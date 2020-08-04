#include "mainwindow.h"
#include <QtWidgets>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QGroupBox>
#include <QPushButton>
#include <QFileDialog>
#include "fbxsdk.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    QLayout * lyt = this->layout();

    QWidget * all = new QWidget;
    QHBoxLayout * hbox = new QHBoxLayout;
    QVBoxLayout * vbox = new QVBoxLayout;

    QPushButton * button = new QPushButton("...");
    edit = new QLineEdit("");
    results = new QTextEdit("");

    connect(button, &QPushButton::released, this, &MainWindow::loadFileName);
    button->resize(button->sizeHint().width(), button->sizeHint().height());

    hbox->addWidget(edit);
    hbox->addWidget(button);
    vbox->addItem(hbox);
    vbox->addWidget(results);
    all->setLayout(vbox);
    lyt->addWidget(all);
}

MainWindow::~MainWindow()
{
}

void MainWindow::loadFileName()
{
    QString fileName = QFileDialog::getOpenFileName(this, ("Open File"), "/home", ("Images (*.fbx)"));
    edit->setText(fileName);
    results->setText(getFileData(fileName));
}

QString MainWindow::getFileData(QString fname)
{
    QString res = "";

    FbxManager * pManager = FbxManager::Create();
    if(!pManager) {
        return "Error creating FbxManager\n";
    }
    FbxIOSettings * ios = FbxIOSettings::Create(pManager, IOSROOT);
    pManager->SetIOSettings(ios);
    FbxString lPath = FbxGetApplicationDirectory();
    pManager->LoadPluginsDirectory(lPath.Buffer());

    FbxScene * pScene = FbxScene::Create(pManager, "My Scene");
    if(!pScene) {
        return "Error creating Scene\n";
    }

    int sdkmajor, sdkminor, sdkrev, filemajor, fileminor, filerev;
    FbxManager::GetFileFormatVersion(sdkmajor, sdkminor, sdkrev);

    FbxImporter * importer = FbxImporter::Create(pManager, "");
    bool status = importer->Initialize(fname.toStdString().c_str(), -1, pManager->GetIOSettings());
    importer->GetFileVersion(filemajor, fileminor, filerev);
    if(!status) {
        return "Error initializing\n";
    }

    if(importer->IsFBX()) {
        res += QString("FBX file version %0.%1.%2\n").arg(filemajor).arg(fileminor).arg(filerev);
        int numAnim = importer->GetAnimStackCount();
        res += QString("  Number of animations %0\n").arg(numAnim);
        for(int i = 0; i < numAnim; i++) {
            FbxTakeInfo* info = importer->GetTakeInfo(i);
            res += QString("    Animation stack %0\n    Name: %1\n    Description: %2").arg(i).arg(info->mName.Buffer()).arg(info->mDescription.Buffer());
        }
    }

//    status = importer->Import(pScene);
//    if(!status) {
//        return "Error importing scene\n";
//    }

    importer->Destroy();

    return res;
}
