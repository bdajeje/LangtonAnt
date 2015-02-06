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
  , m_board { createBoard(m_nbr_cells_x, m_nbr_cells_y) }
{
  reset();

  // Resize the widget
  setFixedSize(500, 500);
}

Board RenderArea::createBoard(unsigned int width, unsigned int height)
{
  Board board;
  board.resize(height);

  for( auto& line : board )
    line.resize(width);

  return board;
}

void RenderArea::fillBoard(Board& board, const QColor& fill_color)
{
  if(board.empty())
    return;

  size_t offset, width;

  for( auto& raw : board )
  {
    width = raw.size();
    for( offset = 0; offset < width; ++offset )
      raw[offset] = fill_color;
  }
}

unsigned int RenderArea::squareWidth() const
{
  return width() / m_nbr_cells_x;
}

unsigned int RenderArea::squareHeight() const
{
  return height() / m_nbr_cells_y;
}

void RenderArea::paintEvent(QPaintEvent * /* event */)
{
  // Create painter
  QPainter painter(this);
  painter.setPen(Qt::black);

  const auto square_width  = squareWidth();
  const auto square_height = squareHeight();

  static const unsigned int end_pos_x = square_width * m_nbr_cells_x;
  static const unsigned int end_pos_y = square_height * m_nbr_cells_y;
  unsigned int pos_y = 0, pos_x;

  // Each raw
  for( size_t y = 0; y < m_nbr_cells_y; ++y )
  {
    pos_x = 0;

    // Each column
    for( size_t x = 0; x < m_nbr_cells_x; ++x )
    {
      // Draw rect color
      painter.fillRect(pos_x, pos_y, square_width, square_height, m_board[y][x]);

      // Draw a column delimiter
      painter.drawLine(pos_x, 0, pos_x, end_pos_y);

      pos_x += square_width;
    }

    // Draw a raw delimiter
    painter.drawLine(0, pos_y, end_pos_x, pos_y);

    pos_y += square_height;
  }

  // Draw last border lines
  painter.drawLine(end_pos_x, 0, end_pos_x, end_pos_y);
  painter.drawLine(0, end_pos_y, end_pos_x, end_pos_y);

  // Draw ant
  painter.fillRect(m_ant_position.x * square_width, m_ant_position.y * square_height,
                   square_width, square_height, Qt::red);
}

void RenderArea::reset()
{
  fillBoard(m_board, Qt::white);

  // Place the ant in the middle of the board
  m_ant_position.x = m_nbr_cells_x / 2;
  m_ant_position.y = m_nbr_cells_y / 2;

  // Make the ant going north on init
  m_ant_direction.x = 0;
  m_ant_direction.y = -1;
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

  // If ant close to borders, enlarge the board
  static unsigned int enlarge_trigger = 2;
  if( m_ant_position.x <= enlarge_trigger                 ||
      m_ant_position.x >= m_nbr_cells_x - enlarge_trigger ||
      m_ant_position.y <= enlarge_trigger                 ||
      m_ant_position.y >= m_nbr_cells_y - enlarge_trigger )
    enlargeBoard();
}

void RenderArea::enlargeBoard()
{
  static unsigned int size = 2;

  // Save current board sizes
  const auto old_nbr_cells_x = m_nbr_cells_x;
  const auto old_nbr_cells_y = m_nbr_cells_y;

  // Update board sizes
  m_nbr_cells_x += size * 2;
  m_nbr_cells_y += size * 2;

  // Create new board with new size
  auto new_board = createBoard(m_nbr_cells_x, m_nbr_cells_y);
  fillBoard(new_board, Qt::white);

  // Put old board data into new one
  for( size_t y = 0; y < old_nbr_cells_y; ++y )
  {
    for( size_t x = 0; x < old_nbr_cells_x; ++x )
      new_board[y + size][x + size] = m_board[y][x];
  }

  // Replace old board by new one
  m_board.swap(new_board);

  // Update ant position
  m_ant_position.x += size;
  m_ant_position.y += size;
}
