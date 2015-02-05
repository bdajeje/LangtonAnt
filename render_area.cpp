#include "render_area.hpp"

#include <QPainter>

// \todo this method sucks
void Direction::turnLeft()
{
  if( x == 0 && y == -1 )
  {
    x = -1;
    y = 0;
  }
  else if( x == 1 && y == 0 )
  {
    x = 0;
    y = -1;
  }
  else if( x == 0 && y == 1 )
  {
    x = 1;
    y = 0;
  }
  else if( x == -1 && y == 0 )
  {
    x = 0;
    y = 1;
  }
}

// \todo this method sucks
void Direction::turnRight()
{
  if( x == 0 && y == -1 )
  {
    x = 1;
    y = 0;
  }
  else if( x == 1 && y == 0 )
  {
    x = 0;
    y = 1;
  }
  else if( x == 0 && y == 1 )
  {
    x = -1;
    y = 0;
  }
  else if( x == -1 && y == 0 )
  {
    x = 0;
    y = -1;
  }
}

RenderArea::RenderArea(unsigned int nbr_cells_x, unsigned int nbr_cells_y, QWidget *parent)
  : QWidget {parent}
  , m_nbr_cells_x {nbr_cells_x}
  , m_nbr_cells_y {nbr_cells_y}
{
  // Create board
  m_board.resize(m_nbr_cells_y);
  for( auto& line : m_board )
    line.resize(m_nbr_cells_x);

  reset();

  // Resize the widget
  setFixedSize({static_cast<int>(m_square_size * m_nbr_cells_x + 2 * m_board_margin),
                static_cast<int>(m_square_size * m_nbr_cells_y + 2 * m_board_margin)});
}

void RenderArea::paintEvent(QPaintEvent * /* event */)
{
  // Create painter
  QPainter painter(this);
  painter.setPen(Qt::black);

  static const unsigned int end_pos_x = m_square_size * m_nbr_cells_x + m_board_margin;
  static const unsigned int end_pos_y = m_square_size * m_nbr_cells_y + m_board_margin;
  unsigned int pos_y, pos_x;

  // Each raw
  for( size_t y = 0; y < m_nbr_cells_y; ++y )
  {
    pos_y = y * m_square_size + m_board_margin;

    // Each column
    for( size_t x = 0; x < m_nbr_cells_x; ++x )
    {
      pos_x = x * m_square_size + m_board_margin;

      // Draw rect color
      painter.fillRect(pos_x, pos_y, m_square_size, m_square_size, m_board[y][x]);

      // Draw a column delimiter
      painter.drawLine(pos_x, m_board_margin, pos_x, end_pos_y);
    }

    // Draw a raw delimiter
    painter.drawLine(m_board_margin, pos_y, end_pos_x, pos_y);
  }

  // Draw last border lines
  painter.drawLine(end_pos_x, m_board_margin, end_pos_x, end_pos_y);
  painter.drawLine(m_board_margin, end_pos_y, end_pos_x, end_pos_y);

  // Draw ant
  auto ant_coordinates = antCoordinates();
  painter.fillRect(ant_coordinates.x + 1, ant_coordinates.y + 1, m_square_size - 1, m_square_size - 1, Qt::red);
}

void RenderArea::reset()
{
  size_t offset;
  for( auto& raw : m_board )
  {
    for( offset = 0; offset < m_nbr_cells_x; ++offset )
      raw[offset] = Qt::white;
  }

  // Place the ant in the middle of the board
  m_ant_position.x = m_nbr_cells_x / 2;
  m_ant_position.y = m_nbr_cells_y / 2;

  // Make the ant going north on init
  m_ant_direction.x = 0;
  m_ant_direction.y = -1;
}

Position RenderArea::antCoordinates() const
{
  return {m_ant_position.x * m_square_size + m_board_margin,
          m_ant_position.y * m_square_size + m_board_margin};
}

const QColor& RenderArea::colorUnderAnt() const
{
  return m_board[m_ant_position.y][m_ant_position.x];
}

void RenderArea::setTileColor(const QColor& color, unsigned int x, unsigned int y)
{
  m_board[y][x] = color;
}

void RenderArea::nextFrame()
{
  // Update tile color under the ant
  // Update ant direction
  if( colorUnderAnt() == Qt::white )
  {
    setTileColor(Qt::black, m_ant_position.x, m_ant_position.y);
    m_ant_direction.turnLeft();
  }
  else
  {
    setTileColor(Qt::white, m_ant_position.x, m_ant_position.y);
    m_ant_direction.turnRight();
  }

  // Move the ant
  m_ant_position.x += m_ant_direction.x;
  m_ant_position.y += m_ant_direction.y;
}
