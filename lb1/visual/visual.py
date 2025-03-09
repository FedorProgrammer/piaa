import json
import matplotlib.pyplot as plt
import matplotlib.patches as patches
import numpy as np
import os
from dataclasses import dataclass

_DEFAULT_OUTPUT_DIR = "visual/output"
_DEFAULT_JSON_PATH = "results.json"


@dataclass
class Square:
    x: int
    y: int
    size: int


@dataclass
class Board:
    size: int
    squares: list[Square]


def _load_data(json_path: str):
    with open(json_path, 'r') as file:
        data = json.load(file)
    file.close()

    if "benchmarks" not in data:
        raise ValueError(
            f"Invalid JSON format: 'benchmarks' key not found in {json_path}")

    if not data["benchmarks"]:
        raise ValueError(f"No benchmark data found in {json_path}")

    return data["benchmarks"]


def _desserialize_square(square_info) -> Square:
    return Square(square_info["x"], square_info["y"], square_info["size"])


def _deserialize_board(board_info) -> Board:
    return Board(board_info["size"], [_desserialize_square(square) for square in board_info["squares"]])


def _configure_ax(ax: plt.Axes, board: Board) -> None:
    ax.set_xlim(0, board.size)
    ax.set_ylim(0, board.size)

    ax.set_xticks(np.arange(0, board.size + 1, 1))
    ax.set_yticks(np.arange(0, board.size + 1, 1))
    ax.grid(True, color='gray', linestyle='-', linewidth=0.5)

    ax.set_title(f"Замощение квадрата размера {board.size}x{board.size}")


def visualize_tiling(board_data, output_dir: str = _DEFAULT_OUTPUT_DIR) -> str:
    os.makedirs(output_dir, exist_ok=True)

    board = _deserialize_board(board_data)

    fig, ax = plt.subplots(figsize=(10, 10))
    _configure_ax(ax, board)

    colors = plt.cm.tab20(np.linspace(0, 1, len(board.squares)))

    for i, square in enumerate(board.squares):
        rect = patches.Rectangle((square.x, board.size-square.y-square.size), square.size, square.size,
                                 linewidth=1, edgecolor='black',
                                 facecolor=colors[i], alpha=0.7)
        ax.add_patch(rect)

        ax.text(square.x + square.size/2, board.size - square.y - square.size/2, str(i+1),
                fontsize=12, ha='center', va='center')

    output_path = os.path.join(output_dir, f"tiling_{board.size}.png")
    plt.savefig(output_path, dpi=150, bbox_inches='tight')
    plt.close()

    return output_path


def plot_times(results, output_dir: str = _DEFAULT_OUTPUT_DIR) -> str:
    sizes = [result["size"] for result in results]
    times = [result["time"] for result in results]

    plt.figure(figsize=(12, 6))
    plt.semilogy(sizes, times, 'o-', linewidth=2, markersize=8)

    for size, time in zip(sizes, times):
        plt.annotate(f'{size}', (size, time), textcoords="offset points",
                     xytext=(0, 5), ha='center')

    plt.grid(True, which='both', linestyle='--', linewidth=0.5)
    plt.title('Сравнение времени выполнения для разных размеров доски')
    plt.xlabel('Размер доски (сторона)')
    plt.ylabel('Время выполнения (секунды)')

    output_path = os.path.join(output_dir, "execution_times.png")
    plt.savefig(output_path, dpi=150, bbox_inches='tight')
    plt.close()

    return output_path


def main(path: str = _DEFAULT_JSON_PATH, output: str = _DEFAULT_OUTPUT_DIR):
    results = _load_data(path)
    print(f"Loaded data for {len(results)} board sizes")
    os.makedirs(output, exist_ok=True)

    for result in results:
        path = visualize_tiling(result, output)
        print(
            f"Created visualization for board size {result['size']} at {path}")

    times_path = plot_times(results, output)
    print(f"Created execution time plot at {times_path}")


if __name__ == "__main__":
    try:
        main()
    except ValueError as e:
        print(f"Error: {e}")
