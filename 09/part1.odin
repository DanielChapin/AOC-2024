package part1

import "core:fmt"
import "core:os"
import "core:strconv"
import "core:strings"

input_path :: proc() -> string {
	if len(os.args) > 1 {
		return os.args[1]
	}

	return `./in.txt`
}

get_input :: proc() -> (files: [dynamic]FileDesc) {
	file_path := input_path()
	data, success := os.read_entire_file_from_filename(file_path)
	if !success {
		panic(fmt.aprintf("Couldn't open file '%s'.", file_path))
	}
	defer delete(data)

	input := strings.trim(string(data), "\r\n\t ")

	is_file := true
	file_id := 0
	for i := 0; i < len(input); i += 1 {
		num, ok := strconv.parse_int(input[i:i + 1])
		assert(ok)

		if is_file {
			append(&files, FileDesc{file_id = file_id, size = num})
			file_id += 1
		} else if num != 0 {
			append(&files, FileDesc{file_id = -1, size = num})
		}
		is_file = !is_file
	}
	return
}

FileDesc :: struct {
	file_id: int,
	size:    int,
}

print_drive :: proc(drive: []FileDesc) {
	for desc in drive {
		if desc.file_id == -1 {
			fmt.printf("_*%d", desc.size)
		} else {
			fmt.printf("%d*%d", desc.file_id, desc.size)
		}
		fmt.print(" ")
	}
	fmt.println()
}

segment_checksum :: proc(file: ^FileDesc, start: int) -> int {
	// \sum_{i = 1}^n = n * (n + 1) / 2
	end := start + file.size
	result := 0
	for i := start; i < end; i += 1 {
		result += i * file.file_id
	}
	return result
	// TODO Not sure where my math is wrong here...
	// return file.file_id * ((end * (end + 1) - start * (start + 1)) / 2)
}

drive_checksum :: proc(drive: []FileDesc) -> int {
	checksum := 0
	position := 0

	for &file in drive {
		if file.file_id != -1 {
			checksum += segment_checksum(&file, position)
		}

		position += file.size
	}

	return checksum
}

compress :: proc(files: []FileDesc) -> (result: [dynamic]FileDesc) {
	drive := files[:]
	for len(drive) > 0 {
		if drive[0].file_id != -1 {
			append(&result, drive[0])
			drive = drive[1:]
			continue
		}

		if drive[0].size == 0 {
			drive = drive[1:]
			continue
		}

		end := len(drive) - 1
		if drive[end].file_id == -1 || drive[end].size == 0 {
			drive = drive[:end]
			continue
		}

		to_move := min(drive[0].size, drive[end].size)
		append(&result, FileDesc{file_id = drive[end].file_id, size = to_move})
		drive[0].size -= to_move
		drive[end].size -= to_move
	}

	return
}

main :: proc() {
	files := get_input()
	defer delete(files)

	compressed := compress(files[:])
	defer delete(compressed)

	fmt.printfln("Checksum: %d", drive_checksum(compressed[:]))
}
