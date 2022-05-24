//
// Created by gusta on 27/01/2022.
//

#include <stdio.h>
#include "storage.h"

static error_t create_template_sdfile(storage_file_t *file, enum line_ref type){

	char sd_filename[10] = { 0 };
	FILE *sdfile = fopen(sd_filename, "w");

	sprintf(sd_filename, "%s.txt", file->filename);

	if(type == ras){
		char line_ref[98] = "pressure_primary, temp1, temp2, temp3, v1, v2, v3, i1, i2, i3, humidity, temp_amb, batt_charge, \n";
		fwrite(line_ref, sizeof(char), sizeof(line_ref), sdfile);
	}

	if(type == obc){
		char line_ref[23] = "obc_data, \n";
		fwrite(line_ref, sizeof(char), sizeof(line_ref), sdfile);
	}

	if(type == gps){
		char line_ref[73] = "day, month, year, second, minute, hour, latitude, longitude, altitude, \n";
		fwrite(line_ref, sizeof(char), sizeof(line_ref), sdfile);
	}

	fclose(sdfile);
	return 0;
}

static uint8_t is_empty(storage_file_t *file){

	FILE *fp;
	fp = fopen(file->filename, "r");
	char c = fgetc(fp);
	if(feof(fp)){
		//is empty
		fclose(fp);
		return 1;
	}
	else{
		fclose(fp);
		return 0;
	}
}

error_t storage_flashfile_init(storage_file_t *file){

		//file not exist
	if(!fopen(file->filename, "r")){
		//create file
		char flash_filename[10] = { 0 };
		FILE *flashfile = fopen(file->filename, "w");

		sprintf(flash_filename, "%s.bin", file->filename);
		fclose(flashfile);
	}

	return 0;
}

error_t storage_sdfile_init(storage_file_t *file, enum line_ref type){
	//Verify if the file already exist
	if(fopen(file->filename, "r") && (is_empty(file))){
		//Exist and its empty
		create_template_sdfile(file, type);
	}
	else{
		//Create a new one
		create_template_sdfile(file, type);
	}

    return 0;
}

error_t storage_log_ras(storage_file_t const *file, ras_blob_t const *data) {


	//Writing in SDCard

    //Concatenate "file_name" + .txt
    char sd_filename[10] = { 0 };
    sprintf(sd_filename, "%s.txt", file->filename);

    FILE *sdfile = fopen(sd_filename, "a");

    char sd_buffer[174] = { 0 }; // 5 Digits before the comma(float)

    sprintf(sd_buffer,
            "%.02e, %.02e, %.02e, %.02e, %.02e, %.02e, %.02e, %.02e, %.02e, %.02e, %.02e, %.02e, %.02e, \n",
            data->values.preassure_primary, data->values.temp_1,
            data->values.temp_2, data->values.temp_3, data->values.v_1,
            data->values.v_2, data->values.v_3, data->values.i_1,
            data->values.i_2, data->values.i_3, data->values.humidity,
            data->values.temp_amb, data->values.batt_charge);

    fwrite(sd_buffer, sizeof(uint8_t), sizeof(sd_buffer), sdfile);
    fclose(sdfile);

    //Writing in FLASH

    char flash_filename[10] = { 0 };
    sprintf(flash_filename, "%s.bin", file->filename);

    FILE *flashfile = fopen(flash_filename, "ab");

    fwrite(data->raw, sizeof(uint8_t), sizeof(ras_blob_values_t), flashfile);

    fclose(flashfile);
    return 0;
}

error_t storage_log_obc(storage_file_t const *file, obc_blob_t const *data) {

	//Writing in SD Card(csv)
    char sd_filename[10] = { 0 };
    sprintf(sd_filename, "%s.txt", file->filename);

    FILE *sdfile = fopen(sd_filename, "a");


    fwrite(data, sizeof(uint8_t), sizeof(data), sdfile);
    fwrite("\n", sizeof(char), 2, sdfile);

    fclose(sdfile);

    //Writing in FLASH
    char flash_filename[10] = { 0 };
    sprintf(flash_filename, "%s.bin", file->filename);

    FILE *flashfile = fopen(flash_filename, "ab");

    fwrite(data, sizeof(uint8_t), sizeof(data), flashfile);

    fclose(flashfile);
    return 0;
}

error_t storage_log_gps(storage_file_t const *file, gps_blob_t const *data) {

	//Writing in SD Card(csv)
    char sd_filename[10] = { 0 };
    sprintf(sd_filename, "%s.txt", file->filename);

    FILE *sdfile = fopen(sd_filename, "a");

    //Day/hour/..
    char data_buffer[78] = { 0 };//4 Digits for day/hour
                                // 5 Digits before decimal(comma)(float)

    sprintf(data_buffer, "%d/%d/%d, %d, %d, %d, %.02e, %.02e, %.02e, \n", data->values.day,
            data->values.month, data->values.year, data->values.second,
            data->values.minute, data->values.hour, data->values.latitude,
            data->values.longitude, data->values.altitude);

    fwrite(data_buffer, sizeof(char), sizeof(data_buffer), sdfile);

    fclose(sdfile);

    //Writing in FLASH
    char flash_filename[10] = { 0 };

    sprintf(flash_filename, "%s.bin", file->filename);

    FILE *flashfile = fopen(flash_filename, "ab");

    fwrite(data->raw, sizeof(uint8_t), sizeof(gps_event_t), flashfile);

    fclose(flashfile);
    return 0;
}
