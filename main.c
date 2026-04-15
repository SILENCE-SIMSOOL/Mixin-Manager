#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>

#define MAX_PATH_LEN 1024
#define BUFFER_SIZE 524288

typedef struct {
	char mixin_dir[MAX_PATH_LEN];
	char mixin_package[MAX_PATH_LEN];
	char json_file[MAX_PATH_LEN];
	char default_target[32];
} Config;

void create_dir(const char* path) { CreateDirectory(path, NULL); }

void save_config(const char* path, Config* cfg) {
	FILE* f = fopen(path, "w");
	if (!f) return;
	fprintf(f, "{\n\t\"mixin_dir\": \"%s\",\n", cfg->mixin_dir);
	fprintf(f, "\t\"mixin_package\": \"%s\",\n", cfg->mixin_package);
	fprintf(f, "\t\"json_file\": \"%s\",\n", cfg->json_file);
	fprintf(f, "\t\"default_target\": \"%s\"\n}", cfg->default_target);
	fclose(f);
}

int load_config(const char* path, Config* cfg) {
	FILE* f = fopen(path, "r");
	if (!f) return 0;
	char line[MAX_PATH_LEN];
	while (fgets(line, sizeof(line), f)) {
		if (strstr(line, "mixin_dir")) sscanf(strstr(line, ":"), ": \"%[^\"]\"", cfg->mixin_dir);
		if (strstr(line, "mixin_package")) sscanf(strstr(line, ":"), ": \"%[^\"]\"", cfg->mixin_package);
		if (strstr(line, "json_file")) sscanf(strstr(line, ":"), ": \"%[^\"]\"", cfg->json_file);
		if (strstr(line, "default_target")) sscanf(strstr(line, ":"), ": \"%[^\"]\"", cfg->default_target);
	}
	fclose(f);
	return 1;
}

void find_files(const char* base_path, const char* current_path, char* output) {
	char search_path[MAX_PATH_LEN];
	sprintf(search_path, "%s\\*", current_path);
	WIN32_FIND_DATA fd;
	HANDLE hFind = FindFirstFile(search_path, &fd);
	if (hFind == INVALID_HANDLE_VALUE) return;

	do {
		if (strcmp(fd.cFileName, ".") == 0 || strcmp(fd.cFileName, "..") == 0) continue;
		char next_path[MAX_PATH_LEN];
		sprintf(next_path, "%s\\%s", current_path, fd.cFileName);

		if (fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
			find_files(base_path, next_path, output);
		} else if (strstr(fd.cFileName, ".java")) {
			char rel[MAX_PATH_LEN];
			strcpy(rel, next_path + strlen(base_path));
			if (rel[0] == '\\' || rel[0] == '/') memmove(rel, rel + 1, strlen(rel));
			for (int i = 0; rel[i]; i++) if (rel[i] == '\\') rel[i] = '.';
			rel[strlen(rel) - 5] = '\0';

			char entry[MAX_PATH_LEN + 10];
			sprintf(entry, "\t\t\"%s\",\n", rel);
			strcat(output, entry);
		}
	} while (FindNextFile(hFind, &fd));
	FindClose(hFind);
}

int main() {
	Config cfg = { 
		"src/main/java/silence/simsool/lucentclient/mixin", 
		"silence.simsool.lucentclient.mixin", 
		"src/main/resources/lucentclient.mixins.json", 
		"client" 
	};

	create_dir("mixinmanager");
	if (!load_config("mixinmanager/config.json", &cfg)) {
		save_config("mixinmanager/config.json", &cfg);
		printf("--------------------------------------------------\n");
		printf("Config created at mixinmanager/config.json\n");
		printf("Please check the paths and run again.\n");
		printf("--------------------------------------------------\n");
		printf("\nPress Enter to exit...");
		getchar();
		return 0;
	}

	char* mixin_list = malloc(BUFFER_SIZE);
	mixin_list[0] = '\0';
	
	printf("Scanning files in: %s\n", cfg.mixin_dir);
	find_files(cfg.mixin_dir, cfg.mixin_dir, mixin_list);

	if (strlen(mixin_list) > 2) {
		mixin_list[strlen(mixin_list) - 2] = '\0';
	}

	FILE* f = fopen(cfg.json_file, "rb");
	if (!f) {
		printf("[ERROR] Could not find JSON file: %s\n", cfg.json_file);
		goto end;
	}
	fseek(f, 0, SEEK_END);
	long fsize = ftell(f);
	fseek(f, 0, SEEK_SET);
	char* content = malloc(fsize + 1);
	fread(content, 1, fsize, f);
	content[fsize] = '\0';
	fclose(f);

	char target_key[64];
	sprintf(target_key, "\"%s\": [", cfg.default_target);
	
	char* start = strstr(content, target_key);
	if (!start) {
		printf("[ERROR] Target section '%s' not found in JSON.\n", target_key);
		free(content);
		goto end;
	}
	
	char* list_start = strchr(start, '[');
	char* list_end = strchr(list_start, ']');
	if (!list_end) {
		printf("[ERROR] Could not find closing bracket ']' for %s\n", cfg.default_target);
		free(content);
		goto end;
	}

	FILE* out = fopen(cfg.json_file, "wb");
	if (!out) {
		printf("[ERROR] Could not write to JSON file.\n");
		free(content);
		goto end;
	}

	fwrite(content, 1, list_start - content + 1, out);
	if (strlen(mixin_list) > 0) {
		fprintf(out, "\n%s\n\t", mixin_list);
	}
	fprintf(out, "%s", list_end);
	fclose(out);

	printf("Successfully updated '%s' section in %s\n", cfg.default_target, cfg.json_file);
	free(content);

end:
	free(mixin_list);
	printf("\nPress Enter to exit...");
	getchar(); 
	return 0;
}