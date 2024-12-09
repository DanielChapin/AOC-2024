def findAllMAS(grid: Array[String]): IndexedSeq[((Int, Int), (Int, Int))] =
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

  val dirs = List((1, 1), (1, -1), (-1, 1), (-1, -1))
  val indices =
    (0 until grid.length).flatMap(row =>
      (0 until grid(row).length()).map(col => (row, col))
    )

  indices
    .flatMap(index => dirs.filter(find(index, _, "MAS")).map((index, _)))

def findXes(
    grid: Array[String]
): Map[(Int, Int), List[((Int, Int), (Int, Int))]] =
  val mases = List.from(findAllMAS(grid))
  def getCenter(pos: (Int, Int), dir: (Int, Int)): (Int, Int) = {
    val (y, x) = pos
    val (dy, dx) = dir
    (y + dy, x + dx)
  }

  mases
    .groupBy(_ match { case (pos, dir) => getCenter(pos, dir) })
    .filter(_ match { case (_, mases) => mases.length > 1 })

@main
def part2(args: String*): Unit =
  val filepath = args match {
    case Seq()         => "./in.txt"
    case Seq(filepath) => filepath
    case _ =>
      throw IllegalArgumentException("Expected either 1 or 0 arguments.")
  }
  val in = input(filepath)
  val result = findXes(in.toArray)
  println(s"Total MAS xes count: ${result.size}")
