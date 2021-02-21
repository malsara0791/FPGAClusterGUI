
int dulitron( float *inputs, volatile float *weights, int num, int STATE)
{
#pragma HLS INTERFACE s_axilite port=num bundle=CTRLBUS
#pragma HLS INTERFACE s_axilite port=STATE bundle=CTRLBUS
#pragma HLS INTERFACE m_axi depth=7840 port=weights offset=slave
#pragma HLS INTERFACE m_axi depth=784 port=inputs offset=slave
#pragma HLS INTERFACE s_axilite port=return bundle=CTRLBUS


	/* Declare variables */

	float total[10] = {0};

	float error[10] = {0};

	float target[10] = {0};

	target[num] = 1.0;

	int i, j;

	/* Calculate total for each perceptron in the network layer */

	int cnt = 0;

	for (j = 0; j < 10; j++)
	{


		for (i = 0; i < 784; i++)
		{

			total[j] = total[j] + (weights[cnt] * inputs[i]);

			cnt = cnt + 1;
		}

		/* Find the error for each perceptron */
		total[j] = total[j] / 784;

		error[j] = target[j] - total[j];
	}

	float largest = total[0];

	for (int n = 0; n < 10; n++)
	{

		if (largest < total[n])
		{
			largest = total[n];
			num = n;
		}
	}

	/* Update the weights with the claculated error */


	if (STATE == 1)
	{
		int ncnt = 0;
		for (j = 0; j < 10; j++)
		{
			for (i = 0; i < 784; i++)
			{
				weights[ncnt] = weights[ncnt] + (0.01 * inputs[i] * error[j]);
				ncnt = ncnt + 1;
			}
		}
	}

	return num;
}

