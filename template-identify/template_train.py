import tensorflow as tf
from dataset import DataSet
import os
import sys

def weight_variable(shape):
    initial = tf.truncated_normal(shape, stddev=0.1)
    return tf.Variable(initial)

def bias_variable(shape):
    initial = tf.constant(0.1, shape=shape)
    return tf.Variable(initial)

def conv2d(x, W):
    return tf.nn.conv2d(x, W, strides=[1, 1, 1, 1], padding='SAME')

def avg_pool_1x2(x):
    return tf.nn.avg_pool(x, ksize=[1, 1, 2, 1], strides=[1, 1, 2, 1], padding='SAME')


sess = tf.InteractiveSession()

nf_conv1 = 4
nf_conv2 = 8
conv1_size = 10
conv2_size = 10
fully_size = 512
data = DataSet(['new_train4'])
x=tf.placeholder(tf.float32, [None, 200], name='input')

y_ = tf.placeholder(tf.float32, [None, 4])

x_image = tf.reshape(x, [-1, 1,200, 1])

W_conv1 = weight_variable([1, conv1_size, 1, nf_conv1])
b_conv1 = bias_variable([nf_conv1])


h_conv1 = tf.nn.relu(conv2d(x_image, W_conv1) + b_conv1)
h_pool1 = avg_pool_1x2(h_conv1)

W_conv2 = weight_variable([1, conv2_size, nf_conv1, nf_conv2])
b_conv2 = bias_variable([nf_conv2])

h_conv2 = tf.nn.relu(conv2d(h_pool1, W_conv2) + b_conv2)
h_pool2 = avg_pool_1x2(h_conv2)

W_fc1 = weight_variable([50 * 1 * nf_conv2, fully_size])
b_fc1 = bias_variable([fully_size])

h_pool2_flat = tf.reshape(h_pool2, [-1, 50*1*nf_conv2])
h_fc1 = tf.nn.relu(tf.matmul(h_pool2_flat, W_fc1) + b_fc1)

keep_prob = tf.placeholder(tf.float32, name="keep_prob")
h_fc1_drop = tf.nn.dropout(h_fc1, keep_prob)

W_fc2 = weight_variable([fully_size, 4])
b_fc2 = bias_variable([4])

y_conv = tf.add(tf.matmul(h_fc1_drop, W_fc2), b_fc2, name = 'y_conv')

cross_entropy = tf.reduce_mean(
        tf.nn.softmax_cross_entropy_with_logits(labels=y_, logits=y_conv))
train_step = tf.train.AdamOptimizer(1e-4).minimize(cross_entropy)
correct_prediction = tf.equal(tf.argmax(y_conv,1, name='output'), tf.argmax(y_,1))

accuracy = tf.reduce_mean(tf.cast(correct_prediction, tf.float32))

sess.run(tf.global_variables_initializer())
for i in range(40000):
    batch = data.next_batch(50)
    if i%1000 == 0:
        train_accuracy = accuracy.eval(feed_dict={x:batch[0], y_: batch[1], keep_prob: 1.0})
        print("step %d, training accuracy %g"%(i, train_accuracy))
    train_step.run(feed_dict={x: batch[0], y_: batch[1], keep_prob: 1.0})

output = open('result.txt', 'a')
output.write('\nnf_conv1: %d, nf_conv2: %d, conv1_size: %d, conv2_size: %d, fully_size: %d\n' % (nf_conv1, nf_conv2, conv1_size, conv2_size, fully_size))
test_data = DataSet(['dataset'])
for tag in test_data.tags:
    batch = test_data.get_by_tag(tag)
    train_accuracy = accuracy.eval(feed_dict={x:batch[0], y_: batch[1], keep_prob: 1.0})
    output.write("tag %s, training accuracy %g\n"%(tag, train_accuracy))

model_path='./checkpoints/model.ckpt'
saver = tf.train.Saver()
save_path = saver.save(sess, model_path)
print("Model saved in file: %s" % save_path)
