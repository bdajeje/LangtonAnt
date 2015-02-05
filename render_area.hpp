#ifndef RENDER_AREA_HPP
#define RENDER_AREA_HPP

#include <vector>

#include <QWidget>

struct Position
{
  Position() = default;

  Position(unsigned int x, unsigned int y)
    : x {x}
    , y {y}
  {}

  unsigned int x {0};
  unsigned int y {0};
};

struct Direction
{
  void turnLeft();
  void turnRight();

  int x {0};
  int y {0};
};

class RenderArea final : public QWidget
{
  public:

    RenderArea(unsigned int nbr_cells_x, unsigned int nbr_cells_y, QWidget *parent = 0);

    void reset();
    void nextFrame();

    const QColor& colorUnderAnt() const;

  private:

    void paintEvent(QPaintEvent * /* event */);
    Position antCoordinates() const;
    void setTileColor(const QColor& color, unsigned int x, unsigned int y);

  private:

    unsigned int m_nbr_cells_x;
    unsigned int m_nbr_cells_y;
    Position m_ant_position;
    Direction m_ant_direction;
    std::vector<std::vector<QColor>> m_board;

    static constexpr unsigned int m_square_size {20};
    static constexpr unsigned int m_board_margin {5};
};

#endif // RENDER_AREA_HPP
