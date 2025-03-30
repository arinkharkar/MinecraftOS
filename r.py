import os

def rename_c_to_cpp(directory):
    for root, _, files in os.walk(directory):
        for file in files:
            if file.endswith(".c"):
                old_path = os.path.join(root, file)
                new_path = os.path.join(root, file[:-2] + ".cpp")
                os.rename(old_path, new_path)
                print(f"Renamed: {old_path} -> {new_path}")

if __name__ == "__main__":
    rename_c_to_cpp(os.getcwd())
