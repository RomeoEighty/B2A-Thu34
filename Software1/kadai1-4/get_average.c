double get_average(const int *numbers, const int length)
{
    double ave = 0.0;
    for (int i = 0; i < length; i++, ave += (double)numbers[i]);
    return ave / (double)length;
}
