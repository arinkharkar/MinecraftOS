import os

def rename_cpp_to_c(directory):
    for root, _, files in os.walk(directory):
        for file in files:
            if file.endswith(".c"):
                old_path = os.path.join(root, file)
                new_path = os.path.join(root, file[:-2] + ".cpp")
                os.rename(old_path, new_path)
                print(f"Renamed: {old_path} -> {new_path}")

if __name__ == "__main__":
    rename_cpp_to_c(os.getcwd())
