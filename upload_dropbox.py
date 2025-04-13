import dropbox
from pathlib import Path

DROPBOX_TOKEN = "sl.u.AFrQnj2zEbvv_eknKU1O-SJW0F-kcsBnUWnUbIRiL5fV7Mj0C_GAkWI8CQHHVknp7JoRYqBOx2ByHPFRJniQDZH0bxgjAKQzhAXPh30-1ve-7GRTyUvpmfq3BpgTmqMMWYsBkyXw51OGQ406kI4dhSJNLx4bj2BVHLT7kffYBmoqLT7cCSZ4q_Tee-T5QlV4Jd_cVOG7fvMop4ugbCsJNI671iZCNXae25NYZ8aB53gcnxstskPLUfk5ZUlhHfwIe27S69OuBUQLu6FVqb2Srd9UFj0O8DOIZwthQ0Cd6wpd-LLSa1ESnGl62elkQFPZdWrF8DTg62g9l7-3xLoCVmr8na929M8Ce9UKOj9Ca9G5MqH8UHP8j9r6lBNPWGl7r8ZKW_XbzOc6MA7hpqvF2jdBcBmfUJwx1Ph-rZ6LHx6R3BReoH9vWUZ4mdaBPwAOz8JnOJFe0rCDWDQvkNXIpbmOolTCORaFym19vlFBwoewaQ87BbmNM6ViSNwPBjXcbBkcfiOH7YM6x4jxBUR05O3_nvUEBgMkgFfnsbPCcDmkcEbTjBQZr3uOIMpq_TrSSzeQA-52POw3PJKew5ciVMJ_Bc3Fvw_f0VAa0oG9Y10CejWQccTUqpVWV4SQFX3JGg0EomqMBv6-_8sJQqUjAWeNn45KB32lYrJv7jEGU1fIJsxFdcsbRZmuSmc4TcxjelgFP4oC_MHy1jrM92TuZsQstj2HRnDulqsjQ5wcjqtbKq1uwqD-SgYffJKf0zgz2k8h01xhUyJj166kcyQyvPqVivguIOCAlb1BWgh4EryoSgRfFeZEarXfZCF7EC8YYMY8Q2cqJO_Gf8tzwHLDzbX9zFW7sPv31B5dxO9uXdDkRiy82HjnWvB0fUDFLR9tSPxhDZInimF-W58iBwCPD_IFGjUJ3KIk5yzNIV4eTIiTHmhWPJKCIlOnzYsJ_F4j3ztZi57gJ7nVSGk48VyuXpYUcI_Iqgp7BmmB9QzjIgjlWNASu80DMzIUpNTumAfCAzShRjzddFVQ2OKWYykBiu7M7uuKcGE58CeR7P50_RxZUwZGcHckBfK9EuMks0H96M4JGQOhKQ2X-3OPqmLpH4W4sBB6Vd7eLfoAltIbITnQ-zBcg6ZKQDiB8FavJpj7ELe_I1d4k0i3t_uLzDwP_HpF46ZolmPlU1vO72iImr4R-lpQ4wn43gHNvpg8sp3q3ARoaUwJ1CROm1eEn9ov5Z61lAGQ8SO9XbWrIk5xu438K_YEY_yfTuJPejv2OwFJ5zgU0zDMVTYnr_0GXn7UHPkMa2c_svxhHxdaGrVwjuCmlgd6InovUwD5Z_vt6XQGoO4W5oWkmgPt51T_Xqj-I2XYn5OEA5R_vY9vF5h5Yox7M47HO0ljK29b7shd8pE5_7elOIzhS9JtV8KJp0o8daBFWuckBpP96czqYSx5gDoZyw"
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