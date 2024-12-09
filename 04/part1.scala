def input(path: String): List[String] =
  scala.io.Source.fromFile(path).getLines().toList

def findAllXMAS(grid: Array[String]): List[((Int, Int), (Int, Int))] =
  def find(pos: (Int, Int), dir: (Int, Int), word: String): Boolean = {
    val (y, x) = pos
    val (dy, dx) = dir
    word match {
      case "" => true
      case word
          if y < 0 ||
            y >= grid.length ||
            x < 0 ||
            x >= grid(y).length ||
            grid(y)(x) != word.head =>
        false
      case word => find((y + dy, x + dx), dir, word.tail)
    }
  }

  val dirs = {
    for
      i <- -1 until 2
      j <- -1 until 2
      if i != 0 || j != 0
    yield (i, j)
  }
  val indices =
    (0 until grid.length).flatMap(row =>
      (0 until grid(row).length()).map(col => (row, col))
    )

  indices
    .flatMap(index => dirs.filter(find(index, _, "XMAS")).map((index, _)))
    .toList

@main
def part1(args: String*): Unit =
  val filepath = args match {
    case Seq()         => "./in.txt"
    case Seq(filepath) => filepath
    case _ =>
      throw IllegalArgumentException("Expected either 1 or 0 arguments.")
  }
  val in = input(filepath)
  val result = findAllXMAS(in.toArray)
  println(s"Total XMAS count: ${result.length}")
