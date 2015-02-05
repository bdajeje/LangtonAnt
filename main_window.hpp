#ifndef MAIN_WINDOW_HPP
#define MAIN_WINDOW_HPP

#include <QMainWindow>
#include <QPushButton>
#include <QSpinBox>
#include <QProgressBar>

#include "render_area.hpp"

class MainWindow final : public QMainWindow
{
  Q_OBJECT

  public:

    MainWindow(QWidget *parent = 0);

  private:

    void render();
    void stopRendering();
    void enableUI();
    void disableUI();

  private slots:

    void toggleRendering();

  private:

    bool m_rendering_started {false};

    QPushButton*  m_button_run;
    QSpinBox*     m_input_steps;
    QProgressBar* m_progress_bar;
    RenderArea*   m_render_area;
};

#endif // MAIN_WINDOW_HPP
