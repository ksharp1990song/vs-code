def read_data_from_file(name):
    with open("test1her.txt", "r") as file:
        lines = file.readlines()
        last_matching_line = None
        for line in lines:
            if line.startswith(name):
                last_matching_line = line
                
        if last_matching_line is not None:
            value = last_matching_line.split(":")[1].strip()
            return value
        else:
            return None