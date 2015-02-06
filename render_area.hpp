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

typedef std::vector<std::vector<QColor>> Board;

class RenderArea final : public QWidget
{
  Q_OBJECT

  public:

    RenderArea(unsigned int nbr_cells_x, unsigned int nbr_cells_y, QWidget *parent = 0);

    void reset();
    void nextFrame();

    const QColor& colorUnderAnt() const;

  private:

    void paintEvent(QPaintEvent * /* event */);
    void setTileColor(const QColor& color, unsigned int x, unsigned int y);
    unsigned int squareWidth() const;
    unsigned int squareHeight() const;

    // \todo put private after testing
    /*! Add specific number of rows and cols around the board
     * \note this method adds line to the top, bottom, left and right.
     *       So calling this method with size=1 will add a total of 4 lines
     */
    void enlargeBoard();

    static Board createBoard(unsigned int width, unsigned int height);
    static void fillBoard(Board& board, const QColor& fill_color);

  private:

    unsigned int m_nbr_cells_x;
    unsigned int m_nbr_cells_y;
    Board m_board;
    Position m_ant_position;
    Direction m_ant_direction;
};

#endif // RENDER_AREA_HPP
