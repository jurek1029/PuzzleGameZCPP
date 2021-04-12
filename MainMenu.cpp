#include "MainMenu.h"
#include <fstream>
#include "Constants.h"
#include <QFileDialog>
#include <QMessageBox>

MainMenu::MainMenu(QWidget *parent) : QMainWindow(parent)
{
    ui.setupUi(this);
    connect(ui.btnPlay, &QPushButton::clicked, this, &MainMenu::OnPlayButtonClicked);
    connect(ui.cBoxSize, QOverload<int>::of (&QComboBox::currentIndexChanged), this, &MainMenu::OnSizeChange);
    connect(ui.checkBoxWithImage, &QCheckBox::stateChanged, this, &MainMenu::OnCheckBoxWithImageStateChanged);
    connect(ui.checkBoxWithNumbers, &QCheckBox::stateChanged, this, &MainMenu::OnCheckBoxWithNumbersStateChanged);
    connect(ui.imgPath, &QLineEdit::textChanged, this, &MainMenu::OnImgPathTextChanged);
    connect(ui.bntImgPahtDialog, &QPushButton::clicked, this, &MainMenu::OnButtonImgPathDialogClicked);
    connect(ui.actionHow_to_play, &QAction::triggered, this, &MainMenu::DisplayHelp);
    LoadBestTimes();
    OnSizeChange(0);
    //
}
/// <summary>
/// Run game with currently selected game size
/// </summary>
void MainMenu::OnPlayButtonClicked()
{
    int size = ui.cBoxSize->currentText().toInt();
    game.Init(size, this);
}

/// <summary>
/// Update current best time for selected size
/// </summary>
/// <param name="index">index of seleted size </param>
void MainMenu::OnSizeChange(int index)
{
    int sec, min;
    sec = best_times[index]/1000 % 60;
    min = best_times[index] / 60000;
    std::string text = "Best time: ";
    text += std::to_string(min);
    text += ":";
    text += std::to_string(sec);
    text += ":";
    text += std::to_string(best_times[index] % 1000);

    ui.lblBestTime->setText(text.c_str());
}

/// <summary>
/// Toggle apriopriate setting, when checkbox 'with Image" is toggled.
/// </summary>
/// <param name="state"></param>
void MainMenu::OnCheckBoxWithImageStateChanged(int state)
{
    ui.checkBoxWithNumbers->setEnabled(state);
    ui.imgPath->setEnabled(state);
    ui.lblImagePath->setEnabled(state);
    ui.bntImgPahtDialog->setEnabled(state);

    if (!state) { // make sure numbers are always on, when displaying without image
        ui.checkBoxWithNumbers->setChecked(true);
        withNumbers = true;
    }
    withImg = state;
}

void MainMenu::OnCheckBoxWithNumbersStateChanged(int state) 
{
    withNumbers = state;
}

void MainMenu::OnImgPathTextChanged(QString)
{
    imgPath = ui.imgPath->text().toStdString();
}

/// <summary>
/// Opens File selection dialog for image.
/// </summary>
void MainMenu::OnButtonImgPathDialogClicked()
{
    QFileDialog dialog(this);
    dialog.setNameFilter(tr("Images (*.png *.jpg)"));
    
    QStringList fileNames;
    if (dialog.exec()) {
        fileNames = dialog.selectedFiles();
        ui.imgPath->setText(fileNames[0]);
        imgPath = fileNames[0].toStdString();
    }
}

/// <summary>
/// Displays game rules and controls in dialog box
/// </summary>
/// <param name="status"></param>
void MainMenu::DisplayHelp(bool status)
{
    QMessageBox message;
    message.setText(\
"    Your goal is to slide tiles to their correct position.\n \
\n \
In case of numbers, they should be sorted from 0 to n^2 - 1 \n \
where tile with lowest number is in the top left corner \n \
and tile with highest number is in the most right down  \n \
                    position\n \n \
        You can move tiles with arrows on your keyboard. \n \
               To automaticly solve puzzle press 's'" );
    message.exec();
}

/// <summary>
/// Loads best times from save file, if file dosen't exist loads default values.
/// </summary>
void MainMenu::LoadBestTimes()
{
    std::ifstream file(TIMES_FILE_PATH, std::ios::binary);
    if (file.is_open()) {
        int len, milisec;
        file >> len;
        for (int i = 0; i < len; ++i) {
            file >> milisec;
            best_times.push_back(milisec);
        }
        file.close();
    }
    else
    {
        for (int i = 0; i < ui.cBoxSize->count(); ++i) {
            best_times.push_back(INT_MAX);
        }
    }

}

/// <summary>
/// Saves Best times to save file, specified in Constants.h
/// </summary>
void MainMenu::SaveBestTimes()
{
    std::ofstream file(TIMES_FILE_PATH, std::ios::binary); //| std::ios::trunc);
    if (file.is_open()) {
        file << best_times.size() << std::endl;
        for (auto it = best_times.begin(); it != best_times.end(); ++it) {
            file << *it << std::endl;
        }
        file.close();
    }
}

void MainMenu::UpdateBestTimes(int size_n, int milisecs)
{
    int index = size_n - SMALEST_GAME_POSIBLE;
    if(best_times[index] > milisecs)
        best_times[index] = milisecs;

    OnSizeChange(index);
}

MainMenu::~MainMenu()
{
    SaveBestTimes();
}

bool MainMenu::isWithImg()
{
    return withImg;
}

bool MainMenu::isWithNumbers()
{
    return withNumbers;
}

std::string MainMenu::GetImgPath()
{
    return imgPath;
}
