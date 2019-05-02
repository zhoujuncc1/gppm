import numpy as np
import auto_template
in_file = 'inter_train.npz'
# out_file = 'ecg_data_normal.npz'
# in_file = 'ecg_data_normal_mv_avg.npz'
data = np.load(in_file)['arr_0'][0][0:100]

#data = normalize(data)
#data = moving_avg_data(data, n=5)
data_avg = np.average(data, axis=0)
#data=data[0]
derivatives= []
for i in range(1, len(data_avg)):
    derivatives.append((data_avg[i] - data_avg[i - 1]))

partitions = auto_template.partition(derivatives, 0.0005)
auto_template.combine_partition(partitions, data_avg, 0.01,min_length=5)
for mark in partitions:
    print(mark)
template = auto_template.template_gen(partitions)


print(template.get_formula())
auto_template.generate_init_prd(template, np.asarray(data))

print(template.get_prds(show_high=True, show_value=True))
print(template.get_constraints())
print(template.get_prds_paramset())
print(template.get_time_bounds())
