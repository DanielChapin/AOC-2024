package part1

import "core:fmt"
import "core:os"

get_input :: proc(filepath: string) -> ([]string, os.Error) {
	data, err := os.read_entire_file_from_filename_or_err(filepath)
    if err {
        return [], err
    }
}

main :: proc() {
	fmt.println("Hello, World!")
}
