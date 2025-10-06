import json

input_file = "new.jsonl"
output_file = "sorted_output.jsonl"

def main():
    with open(input_file, "r") as f:
        lines = [json.loads(line) for line in f]

    lines.sort(key=lambda x: (x["matrix"][0], x["matrix"][1]))

    with open(output_file, "w") as f:
        for item in lines:
            f.write(json.dumps(item) + "\n")

if __name__ == "__main__":
    main()
