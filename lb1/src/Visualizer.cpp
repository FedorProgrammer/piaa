#include "../include/Visualizer.hpp"

#include <matplot/matplot.h>

#include <array>
#include <filesystem>
#include <vector>

#include "../include/Board.hpp"
#include "../include/Deserializer.hpp"
#include "../include/Square.hpp"

#define _TEMP_DIR "temp/"
#define _PLOT_SIZE 600

namespace visualize {

namespace colors {

using color_t = std::array<float, 3>;

const float _alpha = 0.45f;
const float _thickness = 0.5f;

const color_t _red = {1.0f, 0.0f, 0.0f};
const color_t _orange = {1.0f, 0.65f, 0.0f};
const color_t _yellow = {1.0f, 1.0f, 0.0f};
const color_t _green = {0.0f, 1.0f, 0.0f};
const color_t _blue = {0.0f, 0.0f, 1.0f};
const color_t _indigo = {0.29f, 0.0f, 0.51f};
const color_t _violet = {0.93f, 0.51f, 0.93f};
const color_t _magenta = {1.0f, 0.0f, 1.0f};
const color_t _cyan = {0.0f, 1.0f, 1.0f};
const color_t _coral = {1.0f, 0.5f, 0.31f};
const color_t _lime = {0.75f, 1.0f, 0.0f};
const color_t _brown = {0.65f, 0.16f, 0.16f};

const color_t _black = {0.0f, 0.0f, 0.0f};
const color_t _white = {1.0f, 1.0f, 1.0f};

std::vector<color_t> _colors = {_red,  _orange, _yellow, _green,
                                _blue, _indigo, _violet, _magenta,
                                _cyan, _coral,  _lime,   _brown};

color_t _getTransparentColor(const color_t& foreground,
                             const color_t& background, float alpha) {
  return {foreground[0] * alpha + background[0] * (1 - alpha),
          foreground[1] * alpha + background[1] * (1 - alpha),
          foreground[2] * alpha + background[2] * (1 - alpha)};
}

}  // namespace colors

namespace details {

void _configureAxes(matplot::axes_handle ax, int width, int height) {
  ax->xlim({0.0, static_cast<double>(width)});
  ax->ylim({0.0, static_cast<double>(height)});

  auto x_ticks = matplot::linspace(0, width, width + 1);
  auto y_ticks = matplot::linspace(0, height, height + 1);

  ax->x_axis().tick_values(x_ticks);
  ax->y_axis().tick_values(y_ticks);
}

void _drawSquare(const Square& square, colors::color_t fillCollor,
                 colors::color_t borderColor, float thickness) {
  auto rectangle =
      matplot::rectangle(square.x, square.y, square.size, square.size);
  rectangle->color(fillCollor).fill(true);

  auto border =
      matplot::rectangle(square.x, square.y, square.size, square.size);
  border->color(borderColor).fill(false).line_width(thickness);
}

}  // namespace details

std::string visualizeTiling(const Board& board, const std::string& title,
                            const std::string& filename,
                            const std::string& output_dir,
                            const std::string& additional) {
  matplot::figure_handle figure = matplot::figure(true);
  figure->size(_PLOT_SIZE, _PLOT_SIZE);
  figure->position({0, 0, _PLOT_SIZE, _PLOT_SIZE});
  figure->title(title);

  details::_configureAxes(figure->current_axes(), board.getSize(),
                          board.getSize());

  if (additional.length() > 0) {
    figure->current_axes()->title(additional);
  }

  std::vector<Square> squares = board.getSquares();
  for (size_t i = 0; i < squares.size(); ++i) {
    Square& square = squares[i];

    colors::color_t fillColor = colors::_getTransparentColor(
        colors::_colors[i % colors::_colors.size()], colors::_white,
        colors::_alpha);

    square.y = board.getSize() - square.y - square.size;
    details::_drawSquare(square, fillColor, colors::_black, colors::_thickness);
    square.y = (board.getSize() - square.size) / 2;
  }

  std::string output_path = output_dir + filename;

  matplot::save(output_path, "png");
  return output_path;
}

std::string visualizeExecutionTimes(const std::vector<int>& sizes,
                                    const std::vector<double>& times,
                                    const std::string& filename,
                                    const std::string& output_dir) {
  matplot::figure_handle figure = matplot::figure(true);
  figure->size(_PLOT_SIZE * 1.25, _PLOT_SIZE);
  figure->position({0, 0, 1200, 800});

  std::vector<double> sizes_double(sizes.begin(), sizes.end());

  auto p = matplot::semilogy(sizes_double, times, "o-");
  p->line_width(2);
  p->marker_size(8);

  for (size_t i = 0; i < sizes.size(); ++i) {
    matplot::text(sizes_double[i], times[i] * 1.1, std::to_string(sizes[i]));
  }

  auto ax = figure->current_axes();
  ax->grid(true);
  ax->title("Сравнение времени выполнения для разных размеров доски");
  ax->x_axis().label("Размер доски (сторона)");
  ax->y_axis().label("Время выполнения (секунды)");

  std::string output_path = output_dir + filename;
  matplot::save(output_path, "png");

  return output_path;
}

std::string visualizeSteps(const std::vector<Board>& steps,
                           const std::string& output_dir) {
  std::filesystem::create_directory(_TEMP_DIR);

  std::vector<std::string> frame_paths;
  std::string boardSize = std::to_string(steps[steps.size() - 1].getSize());
  int bestCount = INT32_MAX;

  for (size_t i = 0; i < steps.size(); i++) {
    std::string filename = "frame_" +
                           std::string(10 - std::to_string(i).length(), '0') +
                           std::to_string(i) + ".png";
    std::string title =
        "step " + std::to_string(i) + "/" + std::to_string(steps.size() - 1);

    Board step = steps[i];

    if (step.getEmptyCells() == 0) {
      if (step.getSquaresCount() <= bestCount) {
        bestCount = step.getSquaresCount();
      }
    }

    std::string additional =
        "current count: " + std::to_string(step.getSquaresCount()) + " " +
        "current best: ";

    if (bestCount != INT32_MAX)
      additional += std::to_string(bestCount);
    else
      additional += "NS";

    std::string frame_path =
        visualizeTiling(step, title, filename, _TEMP_DIR, additional);
    frame_paths.push_back(frame_path);
  }

  int fps = 2;

  std::string ouput_filename = "steps_" + boardSize + "x" + boardSize;

  auto getPath = [](const std::string& dir, const std::string& filename,
                    const std::string& extenton) -> std::string {
    return dir + filename + extenton;
  };

  std::string gifCommand = "ffmpeg -y -loglevel quiet -framerate " +
                           std::to_string(fps) + " -i " +
                           std::string(_TEMP_DIR) + "frame_%010d.png " +
                           getPath(output_dir, ouput_filename, ".gif");

  std::system(gifCommand.c_str());

  std::string mp4Command = "ffmpeg -y -loglevel quiet -i " +
                           getPath(output_dir, ouput_filename, ".gif") + " " +
                           getPath(output_dir, ouput_filename, ".mp4");

  std::system(mp4Command.c_str());

  std::filesystem::remove_all(_TEMP_DIR);

  return getPath(output_dir, ouput_filename, "");
}

}  // namespace visualize
