import subprocess
import zipfile
import os

folder_to_zip = 'dthreads'
release_files = [f"{folder_to_zip}.zip"]

def zip_folder(folder_path, output_filename):
    folder_name = os.path.basename(folder_path.rstrip('/\\'))
    folder_path = os.path.abspath(folder_path)
    
    with zipfile.ZipFile(output_filename, 'w', zipfile.ZIP_DEFLATED) as zipf:
        for root, dirs, files in os.walk(folder_path):
            for file in files:
                # Create the full file path
                full_path = os.path.join(root, file)
                # Create the archive name by joining the folder name with the relative path
                arcname = os.path.join(folder_name, os.path.relpath(full_path, folder_path))
                zipf.write(full_path, arcname)


# Step 1: Extract the version and description from CHANGE_LOGS.md
def extract_version_and_description():
    with open('CHANGE_LOGS.md', 'r') as file:
        description = []
        version = None
        
        for line in file:
            line = line.strip()
            if not version and line.startswith('## '):
                version = line.split('## ')[1]  # Extract the version title
            if line == "=======":
                break
            description.append(line)
    
    return version, "\n".join(description)

# Step 2: Create and push the Git tag
def create_and_push_tag(version, description):
    try:
        subprocess.run(['git', 'tag', '-a', version, '-m', description], check=True)
        subprocess.run(['git', 'push', 'origin', version], check=True)
        return True
    except subprocess.CalledProcessError as e:
        print(f"Error during git tag/push: {e}")
        return False

# Step 3: Create GitHub release if version contains "stable", "rc", or "beta"
def create_github_release(version, description):
    if any(key in version for key in ["stable", "rc", "beta"]):
        try:
            release_command = ['gh', 'release', 'create', version, *release_files, '--title', version, '--notes', description, '--latest']
            if "stable" not in version:
                release_command.append('--prerelease')
            subprocess.run(release_command, check=True)
            return True
        except subprocess.CalledProcessError as e:
            print(f"Error during GitHub release creation: {e}")
            return False
    else:
        print(f"Version {version} does not match criteria for release. No release will be created.")
        return True

def main():
    version, description = extract_version_and_description()
    
    if not version:
        print("Error: No version found in CHANGE_LOGS.md or invalid format.")
        return
    
    print(f"Extracted version: {version}")
    print(f"Tag description:\n{description}")

    zip_folder(folder_to_zip, f"{folder_to_zip}.zip")
    
    if create_and_push_tag(version, description) and create_github_release(version, description):
        print(f"Successfully created release for version {version}.")
    else:
        print(f"Failed to create or update release for version {version}.")

if __name__ == "__main__":
    main()
