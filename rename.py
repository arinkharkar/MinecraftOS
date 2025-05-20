import os

def rename_c_to_cpp(directory):
    for root, dirs, files in os.walk(directory):
        for filename in files:
            if filename.endswith('.c'):
                old_path = os.path.join(root, filename)
                new_path = os.path.join(root, filename[:-2] + '.cpp')
                os.rename(old_path, new_path)
                print(f"Renamed: {old_path} -> {new_path}")

# Call the function on the 'src' directory
rename_c_to_cpp('src')