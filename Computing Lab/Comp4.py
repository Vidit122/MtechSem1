import mailbox
import json
import os

mbox_files = [f for f in os.listdir('.') if f.endswith('.mbox')]

if not mbox_files:
    print(" No .mbox files found in the current folder.")
    print(" Place your .mbox file in this folder and try again.")
    exit()

print("\n Found the following .mbox files:")
for i, file in enumerate(mbox_files, 1):
    print(f"{i}. {file}")

choice = int(input("\nEnter the number of the file you want to parse: "))
if choice < 1 or choice > len(mbox_files):
    print("Invalid choice. Exiting.")
    exit()

MBOX_FILE = mbox_files[choice - 1]

print(f"\n Selected file: {MBOX_FILE}")
print(" Parsing... Please wait, this may take a while for large files.")

mbox = mailbox.mbox(MBOX_FILE)

emails = []
for message in mbox:
    email_data = {
        "subject": message["subject"],
        "from": message["from"],
        "to": message["to"],
        "date": message["date"],
        "body": ""
    }
    if message.is_multipart():
        for part in message.walk():
            if part.get_content_type() == "text/plain":
                try:
                    email_data["body"] += part.get_payload(decode=True).decode(errors="ignore")
                except:
                    pass
    else:
        try:
            email_data["body"] = message.get_payload(decode=True).decode(errors="ignore")
        except:
            pass

    emails.append(email_data)

OUTPUT_FILE = "emails.json"
with open(OUTPUT_FILE, "w", encoding="utf-8") as f:
    json.dump(emails, f, indent=4)

print(f"\n Done! Extracted {len(emails)} emails and saved them to {OUTPUT_FILE}")
