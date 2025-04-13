import dropbox
from pathlib import Path

DROPBOX_TOKEN = "### insert dropbox api ###"
LOCAL_FILE = "faculty_contact.txt"
DROPBOX_PATH = "/faculty_contact.txt"

def upload_to_dropbox():
    try:
        dbx = dropbox.Dropbox(DROPBOX_TOKEN)
        
        if not Path(LOCAL_FILE).exists():
            raise FileNotFoundError(f"{LOCAL_FILE} not found")
        
        with open(LOCAL_FILE, "rb") as f:
            dbx.files_upload(
                f.read(),
                DROPBOX_PATH,
                mode=dropbox.files.WriteMode("overwrite")
            )
        print("Successfully uploaded to Dropbox!")
        
    except Exception as e:
        print(f"Error: {str(e)}")

if __name__ == "__main__":
    upload_to_dropbox()
