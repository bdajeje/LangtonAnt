#include "main_window.hpp"

#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QThread>

MainWindow::MainWindow(QWidget *parent)
  : QMainWindow(parent)
{
  auto central_widget = new QWidget;

  // Layouts
  auto main_layout   = new QVBoxLayout(central_widget);
  auto button_layout = new QHBoxLayout;

  // Widgets
  m_button_run   = new QPushButton;
  m_input_steps  = new QSpinBox;
  m_input_speed  = new QSpinBox;
  m_progress_bar = new QProgressBar;
  m_render_area  = new RenderArea(7, 7);

  // Default widgets values
  m_input_steps->setRange(1, 10000);
  m_input_steps->setValue(1000);
  m_input_steps->setToolTip("Set how many steps to generate");
  m_input_steps->setPrefix("Number steps: ");
  m_input_speed->setValue(25);
  m_input_speed->setRange(1, 1000);
  m_input_speed->setToolTip("Sleep time between two iterations in milliseconds");
  m_input_speed->setPrefix("Sleep time: ");
  m_input_speed->setSuffix(" ms");
  m_button_run->setToolTip("Start / Stop the rendering");
  m_progress_bar->setTextVisible(true);

  // Placements
  button_layout->addWidget(m_input_steps);
  button_layout->addWidget(m_input_speed);
  button_layout->addWidget(m_button_run);
  main_layout->addLayout(button_layout);
  main_layout->addWidget(m_progress_bar);
  main_layout->addWidget(m_render_area);
  setCentralWidget(central_widget);

  enableUI();

  // Widget connections
  connect(m_button_run, SIGNAL(pressed()), this, SLOT(toggleRendering()));
}

void MainWindow::toggleRendering()
{
  if(m_rendering_started)
  {
    stopRendering();
    enableUI();
  }
  else
  {
    m_render_area->reset();
    disableUI();
    m_progress_bar->setValue(0);
    m_progress_bar->setMaximum(m_input_steps->value());
    render(m_input_speed->value(), m_input_steps->value());
  }
}

void MainWindow::stopRendering()
{
  m_rendering_started = false;
}

void MainWindow::enableUI()
{
  m_button_run->setText("Start");
  m_input_steps->setDisabled(false);
}

void MainWindow::disableUI()
{
  m_button_run->setText("Stop");
  m_input_steps->setDisabled(true);
}

// \todo put this method in another thread so it doesn't block the UI
void MainWindow::render(int sleep_time, int nbr_steps)
{
  m_rendering_started = true;
  for( int step = 0; step < nbr_steps; ++step )
  {
    if(step != 0)
      QThread::msleep(sleep_time);

    m_progress_bar->setValue(m_progress_bar->value() + 1);
    m_render_area->nextFrame();
    m_render_area->repaint();

    // Does the user want the rendering to stop
//    if(m_rendering_started)
//      break;
  }
}
