#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <locale.h>
#include <stdlib.h>
#include <wchar.h>
#include <time.h>
#include <conio.h>

typedef struct {
    int cpu_load;
    int ram_load;
    int disk_load;
} InputValues;

static int CPU_LIMIT = 0;
static int RAM_LIMIT = 0;
static int DISK_LIMIT = 0;

void SetCheckValues() {
    printf("������� ��������� �������� ��� �������� ���������� (0-100): ");
    scanf("%d", &CPU_LIMIT);
    printf("������� ��������� �������� ��� �������� ����������� ������ (0-100): ");
    scanf("%d", &RAM_LIMIT);
    printf("������� ��������� �������� ��� �������� ��������� ������������ (0-100): ");
    scanf("%d", &DISK_LIMIT);
    printf("��������� �������� �����������.\n");
}

void CheckThreatLevel(InputValues values) {
    int below_threshold = 0;

    if (values.cpu_load > CPU_LIMIT) {
        below_threshold++;
    }
    if (values.ram_load > RAM_LIMIT) {
        below_threshold++;
    }
    if (values.disk_load > DISK_LIMIT) {
        below_threshold++;
    }

    switch (below_threshold) {
    case 3:
        wprintf(L"������� ������: ������� (��� �������� ���� ������)\n\n");
        break;
    case 2:
        wprintf(L"������� ������: ������� (��� �������� ���� ������)\n\n");
        break;
    case 1:
        wprintf(L"������� ������: ������ (���� �������� ���� ������)\n\n");
        break;
    default:
        wprintf(L"��� �������� � �������� �����\n\n");
        break;
    }
}

void ReadFromLogFile() {
    FILE* log_file = _wfopen(L"log.txt", L"r, ccs=UTF-8");
    if (!log_file) {
        fopen(L"log.txt", L"w, ccs=UTF-8");
    }

    InputValues values;
    int count = 0;
    wchar_t line[256];

    while (fgetws(line, sizeof(line) / sizeof(wchar_t), log_file)) {
        values.cpu_load = -1;
        values.ram_load = -1;
        values.disk_load = -1;

        wchar_t* cpu_str = wcsstr(line, L"�������� ����������");
        wchar_t* ram_str = wcsstr(line, L"�������� ����������� ������");
        wchar_t* disk_str = wcsstr(line, L"�������� ��������� ������������");

        if (cpu_str) {
            swscanf(cpu_str, L"�������� ���������� = %d%%", &values.cpu_load);
        }
        if (ram_str) {
            swscanf(ram_str, L"�������� ����������� ������ = %d%%", &values.ram_load);
        }
        if (disk_str) {
            swscanf(disk_str, L"�������� ��������� ������������ = %d%%", &values.disk_load);
        }

        if (values.cpu_load != -1 || values.ram_load != -1 || values.disk_load != -1) {
            count++;
            wprintf(L"�������� ����������: %d%%\n", values.cpu_load);
            wprintf(L"�������� ����������� ������: %d%%\n", values.ram_load);
            wprintf(L"�������� ��������� ������������: %d%%\n", values.disk_load);
            CheckThreatLevel(values);
        }
        else {
            wprintf(L"��������� �������� �� ���� ������� �� ������: %s\n", line);
        }
    }

    fclose(log_file);

    if (count == 0) {
        wprintf(L"�� ������� ������� ������ �� �����. ���������, ��� ������ ����������.\n");
    }
}

void GenerateLogFile(int entries) {
    FILE* log_file = _wfopen(L"log.txt", L"w, ccs=UTF-8");
    if (!log_file) {
        perror("�� ������� ������� ����");
        exit(EXIT_FAILURE);
    }

    srand((unsigned int)time(NULL));

    for (int i = 0; i < entries; i++) {
        int cpu_load = rand() % 101;
        int ram_load = rand() % 101;
        int disk_load = rand() % 101;

        int order = rand() % 6;

        switch (order) {
        case 0:
            fwprintf(log_file, L"�������� ���������� = %d%%, �������� ����������� ������ = %d%%, �������� ��������� ������������ = %d%%\n",
                cpu_load, ram_load, disk_load);
            break;
        case 1:
            fwprintf(log_file, L"�������� ����������� ������ = %d%%, �������� ���������� = %d%%, �������� ��������� ������������ = %d%%\n",
                ram_load, cpu_load, disk_load);
            break;
        case 2:
            fwprintf(log_file, L"�������� ��������� ������������ = %d%%, �������� ���������� = %d%%, �������� ����������� ������ = %d%%\n",
                disk_load, cpu_load, ram_load);
            break;
        case 3:
            fwprintf(log_file, L"�������� ����������� ������ = %d%%, �������� ��������� ������������ = %d%%, �������� ���������� = %d%%\n",
                ram_load, disk_load, cpu_load);
            break;
        case 4:
            fwprintf(log_file, L"�������� ���������� = %d%%, �������� ��������� ������������ = %d%%, �������� ����������� ������ = %d%%\n",
                cpu_load, disk_load, ram_load);
            break;
        case 5:
            fwprintf(log_file, L"�������� ��������� ������������ = %d%%, �������� ����������� ������ = %d%%, �������� ���������� = %d%%\n",
                disk_load, ram_load, cpu_load);
            break;
        }
    }

    fclose(log_file);
    wprintf(L"���-���� ������� ������ � %d ��������.\n", entries);
}

int main() {
    setlocale(LC_CTYPE, "");
    /*const wchar_t* filename = L"log.txt";*/
    char choice;
    int num_of_logs;
    do {

        printf("�������� ����������� �������:\n");
        printf("1) ������� ��������� ��������\n2) ������ ���-�����\n3) ������������� ������ ��� ���-�����\n��� ������ �� ��������� ������� �� 0\n");
        choice = _getch();
        system("cls");
        switch (choice)
        {
        case '1':
            SetCheckValues();
            break;
        case '2':
            ReadFromLogFile();
            break;
        case '3':
            printf("������� ���������� �����: ");
            scanf("%d", &num_of_logs);
            GenerateLogFile(num_of_logs);
            break;
        case '0':
            break;
        default:
            printf("��� ����� �������.\n");
        }
    } while (choice != '0');
    return 0;
}