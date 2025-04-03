import os

def rename_files(root_dir):
    for dirpath, _, filenames in os.walk(root_dir):
        for filename in filenames:
            if filename.endswith(".c.c"):
                old_path = os.path.join(dirpath, filename)
                new_filename = filename[:-2]  # Remove the last ".c"
                new_path = os.path.join(dirpath, new_filename)
                
                try:
                    os.rename(old_path, new_path)
                    print(f"Renamed: {old_path} -> {new_path}")
                except Exception as e:
                    print(f"Error renaming {old_path}: {e}")

if __name__ == "__main__":
    current_directory = os.getcwd()
    rename_files(current_directory)