# ***************************************************************************************
#    Project: dthreads -> https://github.com/dezashibi-c/dthreads
#    File: tag.py
#    Date: 2024-08-14
#    Author: Navid Dezashibi
#    Contact: navid@dezashibi.com
#    Website: https://www.dezashibi.com | https://github.com/dezashibi
#    License:
#        `tag.py` -> Simple Utility For Making Releases and Tags for github     
#        BSD 3-Clause License
#
#        Copyright (c) 2024, Navid Dezashibi <navid@dezashibi.com> All rights reserved.
#
#        Redistribution and use in source and binary forms, with or without
#        modification, are permitted provided that the following conditions are met:
#
#        1. Redistributions of source code must retain the above copyright notice,
#           this list of conditions and the following disclaimer.
#
#        2. Redistributions in binary form must reproduce the above copyright notice,
#           this list of conditions and the following disclaimer in the documentation
#           and/or other materials provided with the distribution.
#
#        3. Neither the name of the copyright holder nor the names of its contributors
#           may be used to endorse or promote products derived from this software
#           without specific prior written permission.
#
#        THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
#        ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
#        WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
#        IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
#        INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
#        BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
#        DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
#        LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE
#        OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED
#        OF THE POSSIBILITY OF SUCH DAMAGE.
# ***************************************************************************************
#    Description: The idea is to run this simple script after a few modification to create 
#                 tag and also release in case certain criteria are met.
#
#                 The script looks for `CHANGE_LOGS.md` in the current working directory
#                 it assumes the mentioned file has the following format
#                 
# ## vx.y.z[-beta|rc|stable] <- the `## ` sequence is mandatory
# 
# <any text>
# 
# ======= <-- this is mandatory, it's 7 equal signs
# 
# ## vx.y.z[-beta|rc|stable]
# 
# <any text>
# 
# =======
# 
# ## vx.y.z[-beta|rc|stable]
# 
# <any text>
#
#
#                 The scripts opens the file, get the version from the first line, reads
#                 until it gets the 7 equal signs or EOF.
#                 if the version has no "rc", "beta", "stable" no release will be generated
#                 only a proper tag will be added. but in case it's "stable" a release with
#                 marked as latest will be generated otherwise a release marked as pre-
#                 production will be generated. and in case a release is make any files
#                 mentioned in "release_files" list will be added to the release too.
# ***************************************************************************************

# name of the folder to zip this path is relative to the current working directory
# it means if you run it from the root folder it looks for the folder to be zipped
# in the root folder
# leave it empty string `''` for disabling zip
folder_to_zip = 'dthreads'

# you can list name of file or files you would like to be added individually to github
# releases
release_files = [f"{folder_to_zip}.zip"]


#########################################################################################
## DO NOT MAKE ANY CHANGES BELOW
#########################################################################################

import subprocess
import zipfile
import os

def zip_folder(folder_path, output_filename):
    folder_name = os.path.basename(folder_path.rstrip('/\\'))
    folder_path = os.path.abspath(folder_path)
    
    with zipfile.ZipFile(output_filename, 'w', zipfile.ZIP_DEFLATED) as zipf:
        for root, dirs, files in os.walk(folder_path):
            for file in files:
                full_path = os.path.join(root, file)
                arcname = os.path.join(folder_name, os.path.relpath(full_path, folder_path))
                zipf.write(full_path, arcname)


def extract_version_and_description():
    with open('CHANGE_LOGS.md', 'r') as file:
        description = []
        version = None
        
        for line in file:
            line = line.strip()
            if not version and line.startswith('## '):
                version = line.split('## ')[1]
            if line == "=======":
                break
            description.append(line)
    
    return version, "\n".join(description)

def create_and_push_tag(version, description):
    try:
        subprocess.run(['git', 'tag', '-a', version, '-m', description], check=True)
        subprocess.run(['git', 'push', 'origin', version], check=True)
        return True
    except subprocess.CalledProcessError as e:
        print(f"Error during git tag/push: {e}")
        return False

def create_github_release(version, description):
    if any(key in version for key in ["stable", "rc", "beta"]):
        try:
            if folder_to_zip != '':
                zip_folder(folder_to_zip, f"{folder_to_zip}.zip")

            release_command = ['gh', 'release', 'create', version, *release_files, '--title', version, '--notes', description]
            if "stable" not in version:
                release_command.append('--prerelease')
            else:
                release_command.append('--latest')
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
    
    if create_and_push_tag(version, description) and create_github_release(version, description):
        print(f"Successfully created release for version {version}.")
    else:
        print(f"Failed to create or update release for version {version}.")

if __name__ == "__main__":
    main()
