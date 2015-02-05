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
  m_progress_bar = new QProgressBar;
  m_render_area  = new RenderArea(25, 25);

  // Default widgets values
  m_input_steps->setValue(10);
  m_input_steps->setRange(1, 1000);
  m_input_steps->setToolTip("Set how many steps to generate");
  m_input_steps->setPrefix("Number steps: ");
  m_button_run->setToolTip("Start / Stop the rendering");
  m_progress_bar->setTextVisible(true);

  // Placements
  button_layout->addWidget(m_input_steps);
  button_layout->addWidget(m_button_run);
  main_layout->addLayout(button_layout);
  main_layout->addWidget(m_progress_bar);
  main_layout->addWidget(m_render_area);
  setCentralWidget(central_widget);

  enableUI();

  // Widget connections
  connect(m_button_run, SIGNAL(pressed()), this, SLOT(toggleRendering()));
  connect(m_input_steps, SIGNAL(valueChanged(int)), m_progress_bar, SLOT(setMaximum(int)));
}

void MainWindow::toggleRendering()
{
  m_progress_bar->setValue(0);

  if(m_rendering_started)
  {
    stopRendering();
    enableUI();
  }
  else
  {
    m_render_area->reset();
    disableUI();
    render();
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

void MainWindow::render()
{
  m_rendering_started = true;

  const auto nbr_steps = m_input_steps->value();
  for( int step = 0; step < nbr_steps; ++step )
  {
    if(step != 0)
      QThread::msleep(25);

    m_progress_bar->setValue(m_progress_bar->value() + 1);
    m_render_area->nextFrame();
    m_render_area->repaint();
  }
}
