#pragma once
#include "Game.h"
#include <QtWidgets/QMainWindow>
#include "ui_MainMenu.h"
#include <QString>


class MainMenu : public QMainWindow
{
    Q_OBJECT

public:
    MainMenu(QWidget *parent = Q_NULLPTR);
    void UpdateBestTimes(int size_n, int milisecs);
    ~MainMenu();
    bool isWithImg();
    bool isWithNumbers();
    std::string GetImgPath();

private:
    bool withImg = false, withNumbers = true;
    std::string imgPath;
    Ui::MainMenuClass ui;
    Game game;
    std::vector<int> best_times;
    void OnPlayButtonClicked();
    void OnSizeChange(int index);
    void LoadBestTimes();
    void SaveBestTimes();
    void OnCheckBoxWithImageStateChanged(int);
    void OnCheckBoxWithNumbersStateChanged(int);
    void OnImgPathTextChanged(QString s);
    void OnButtonImgPathDialogClicked();
    void DisplayHelp(bool);
};
