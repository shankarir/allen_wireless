"""
memory files obtained from copying gdb output must be first processed.

In vim, use regex command "%s/.\+:\s\+//g" to delete the address/pointer field.

Edit 11/15/2015: No longer need the regex command, just save gdb.txt to whatever and run.
                 The regex is taken care of in load_mem_values

Edit 1/6/2015: Note that the amplitude of the signal needs to be a bit higher to see values clearly.

Use within_amp() for plotting intan_setup_test.asm, intan_setup_test2.asm results. Assumed dumped gdb.txt
                 values include Intan setup results.
    within_amp_32ch() for IIR_oscillator_test. Assume dumped gdb.txt contains only data values.

"""
import numpy as np
import matplotlib.pyplot as plt
import scipy.io

'''
Convert the ADC output to voltage values:
The outputs are in binary offset, where 0x0000 corresponds to 0V, and 0xffff = 3.37V,
when connected to JTAG.

The amps have a gain of 196. So to get the actual input value in mV, we do:

    (3.37/65535)*val/196*1000
where 65535=0xffff.

'''
VH = 3.37
gain = 196
scaler = VH/0xffff/196*1000 # convert to mV

def convert_to_signed(val, nbits):
    '''
    Convert an integer to it's two's complement value:
    e.g. 255 is 0xff, which is two's complement is 128
    '''
    if val > 2**(nbits-1)-1:
        return (2**nbits-val)*-1
    else:
        return val

def load_mem_values(fname):
    # Read in the hex values, save in array data
    f = open(fname, 'r')

    data = []
    for line in f.readlines():
        for val in ((line.split(':'))[1].replace('\n','').split('\t'))[1:]:
            data.append(int(val, 16))

    f.close()
    return data

def to16bits(n):
    '''
    Given a 32-bits word, split and return the two 16-bits word.
    Return order is the lower word, followed by the higher word
    '''
    return ((n & 0xffff),(n >> 16 & 0xffff))

def convert_mem_values(data):
    new_data = [];
    for i in data:
        new_data.extend(to16bits(i))
    return new_data

def check_setup_values(data):
    '''
    Every memory test, the response to the Intan-setup sequence should return for each
    amplifier the following, in order:
    1. [rubbish, rubbish, rubbish] - the first two frame-sync cycles output whatever is
       in Intan's pipeline. The third one is a repeat of the first command, which often glitches
       and is sent again.
    2. [0xffde, 0xff02, 0xff04, 0xff00, 0xff80,0xff00, 0xff00, 0xff00, 0xff16, 0xff00, 0xff17, 0xff00, 
        0xff15, 0xff00, 0xffff, 0xffff, 0xffff, 0xffff] - These are the return values in response
       to writing to the Intan registers.
       Edit: Need to match the REG0-17 lower byte values in intan_setup_test files
    3. [0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000] - Values in
       response to the calibration command and the 9 dummy commands proceeding it.
    4. [0x0049, 0x004e, 0x0054, 0x0041, 0x004e] - Reading reg40-44, which spells out INTAN in ASCII.

    Input is the entire memory content.
    Output the memory content, minus the first 36 set of values.
    '''
    rubbish_length = 3
    n_amp = 4
    setup_list = [0xffde, 0xff02, 0xff04, 0xff00, 0xff94, 0xff00, 0xff00, 0xff00, 0xff11, 0xff00, 0xff10, 0xff00, \
                  0xff11, 0xff00, 0xffff, 0xffff, 0xffff, 0xffff]
    calib_list = [0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000]
    intan_list = [0x0049, 0x004e, 0x0054, 0x0041, 0x004e]

    master_list = list(setup_list)
    master_list.extend(calib_list)
    master_list.extend(intan_list)

    wanted = data[rubbish_length*n_amp : len(master_list)*4 + rubbish_length*n_amp]
    i = 0   # index master_list
    while i < len(master_list):
        memvals = [wanted[k] for k in [j+i*n_amp for j in range(n_amp)] ]
        if all(val==master_list[i] for val in memvals):
            i = i+4
        else:
            print "Setup values incorrect! Expected %s, %s\n" % \
                    (hex(master_list[i]), [hex(val) for val in memvals])
            i = i+4

    return data[36*4:]

def plot_mem_vals1(data):
    '''
    This method assumes we are getting samples for 1 period of f, for a fixed channel
    in n amplifiers. Each sample is 16-bits. Therefore, we need to examine a total of:

        156 sample/period x 4 amps = 624 samples,

    from the input data, if we are sampling a 6.4kHz wave with 1MHz, from 4 amps.
    
    Every set of four values are in order of amp1, 2, 3, and 4.
    '''
    f = 6.4*10**3   # freq of signal
    f_s = 10**6     # 1MHz sampling freq
    sample_per_period = int(f_s/f)  # closest integer sample amount
    n_periods = 2
    n_amp = 4
    amp1 = [data[i] for i in range(sample_per_period*n_amp*n_periods) if i%4==0]
    amp2 = [data[i] for i in range(sample_per_period*n_amp*n_periods) if i%4==1]
    amp3 = [data[i] for i in range(sample_per_period*n_amp*n_periods) if i%4==2]
    amp4 = [data[i] for i in range(sample_per_period*n_amp*n_periods) if i%4==3]

    
    amp1 = [i*scaler for i in amp1]
    amp2 = [i*scaler for i in amp2]
    amp3 = [i*scaler for i in amp3]
    amp4 = [i*scaler for i in amp4]
    
    xx = range(len(amp1))
    p, (ax1, ax2, ax3, ax4) = plt.subplots(4, sharex=True, sharey=True)
    ax1.plot(xx, amp1, color='blue')
    ax2.plot(xx, amp2, color='red')
    ax3.plot(xx, amp3, color='green')
    ax4.plot(xx, amp4, color='black')
    ax1.set_ylabel('Amp1 ch0 (mV)')
    ax2.set_ylabel('Amp2 ch0 (mV)')
    ax3.set_ylabel('Amp3 ch0 (mV)')
    ax4.set_ylabel('Amp4 ch0 (mV)')
    ax4.set_xlabel('Time (uS)')
    plt.show(block=False)

    return (amp1, amp2, amp3, amp4)

def plot_mem_vals2(data, n_channels, f, n_periods=2, truncation=False, signed=False):
    '''
    Similar to plot_mem_vals1, except we are plotting all 32 channels within the same amp!
    Might be slow with lots of samples because I'm not allocating memory, but I don't plan to
    get too many samples!
    '''
    f_max = 10.0**6     # 1MHz sampling freq
    f_s = f_max/n_channels
    sample_per_period = int(np.ceil(f_s/f))
    n_samples = n_periods*sample_per_period*n_channels

    channels = [list() for i in xrange(n_channels)]
    if not truncation:
        for ch in xrange(n_channels):
            channels[ch] = [data[i] for i in range(int(sample_per_period*n_channels*n_periods)) if i%n_channels==ch]
    else:
        # we are taking only the most significant byte out of 16bits as samples
        # no scaler, offset binary is enough
        for ch in xrange(n_channels):
            channels[ch] = [(data[i]>>8) for i in range(int(sample_per_period*n_channels*n_periods)) if i%n_channels==ch]
    
    if truncation:
        nbits = 8
    else:
        nbits = 16
    if signed:
        channels = [ [convert_to_signed(i, nbits) for i in ch] for ch in channels]
        
    maxes = [max(i) for i in channels]
    mins = [min(i) for i in channels]

    
    # plot all these...6 rows of 6
    xx = range(len(channels[0]))
    plt.figure()
    ncols = np.ceil(np.sqrt(n_channels))
    nrows = np.ceil(n_channels/ncols)
    for ch in xrange(n_channels):
        plt.subplot(nrows, ncols, ch+1)
        plt.plot(xx, channels[ch], 'b-*')
        plt.ylim(min(mins)-1, max(maxes)+1)
        if ch % 6 == 0:
            t = 'Channel%d hex' % (ch)
            plt.ylabel(t)

    plt.show(block=False)
    return channels

def plot_mem_32ch(data, truncation=False, signed=False):
    '''
    Similar to plot_mem_vals2, but we dont care how many periods are there,
    Assume data comes from 32 channels. The first sample is from ch0, based on intan_setup_test2 firmware setup.
    assign samples to channels round-robin style.

    Sample is offset binary
    '''
    channels = [list() for i in xrange(32)]
    ch = 0
    for i in xrange(len(data)):
        if not truncation:
            channels[ch].append(data[i])
        else:
            channels[ch].append( (data[i]>>8) )
        ch = (ch+1)%32
    
    if truncation:
        nbits = 8
    else:
        nbits = 16
    if signed:
        channels = [ [convert_to_signed(i, nbits) for i in ch] for ch in channels]

    maxes = [max(i) for i in channels]
    mins = [min(i) for i in channels]
    lens = [len(ch) for ch in channels]
    
    plt.figure()
    ncols = 6
    nrows = 6
    for ch in xrange(32):
        plt.subplot(nrows, ncols, ch+1)
        xx = range(len(channels[ch]))
        plt.plot(xx, channels[ch], 'b-*')
        plt.ylim(min(mins)-1, max(maxes)+1)
        if ch % 6 == 0:
            t = 'Channel%d hex' % (ch)
            plt.ylabel(t)

    plt.show(block=False)
    return channels

def plot_mem_vals3(data):
    '''
    Plot everything as a single channel
    '''
    nsamples = int(1.5*156*32)
    samples = [i*scaler for i in data]
    print "len of samples is ", len(samples)
    xx = range(nsamples)
    plt.figure()
    plt.plot(xx, samples[0:nsamples], color='blue')
    plt.ylabel('Signal (mV)')
    plt.xlabel('Time (uS)')
    plt.show(block=False)
    return samples

def saveCh_to_mat(data, fname):
    '''
    data is of the structure [ch1, ch2, ch3,...]
    where ch1, ch2, etc is a list of values.

    Convert each list into a matlab array and save all into a mat file
    '''
    mat = {}
    for i in xrange(len(data)):
        name = 'ch%d' % i
        mat[name] = np.array(data[i])
    scipy.io.savemat(fname, mdict=mat)

#Code for analyzing between amps
def between_amps(fname):
    data = load_mem_values(fname)
    data = convert_mem_values(data)
    data = check_setup_values(data)
    return plot_mem_vals1(data)
    
#Code for analyzing channels within an amp
def within_amp(fname, n_channels, f, n_periods=2, truncation=False, signed=False):
    '''
    fname = saved memory dump.
    n_channels = number of channels we are plotting/saving.
    f = frequency of the signal applied.
    truncation: True if we are plotting the MSByte of the results.
    '''
    data = load_mem_values(fname)
    data = convert_mem_values(data)
    data = check_setup_values(data)
    data = plot_mem_vals2(data, n_channels, f, n_periods, truncation, signed)
    # plot ch0 and ch31 together for comparison
    plt.figure()
    xx = range(len(data[0]))
    plt.plot(xx, data[0], 'b-*')
    plt.plot(xx, data[-1], 'r-*')
    plt.ylim(min(min(data[0]),min(data[-1]))-1, max(max(data[0]),max(data[-1]))+1)
    plt.legend(['Ch0','Ch31'])
    plt.show(block=False)
    return data

def within_amp_32ch(fname, truncation=False, signed=False):
    '''
    Same as within_amp, assumes 32 channels, and doesn't care about number
    of periods.

    Used with IIR_oscillator_test. Since it assumes the dumped values do not
    contain setup values.
    '''
    data = load_mem_values(fname)
    data = convert_mem_values(data)
    #data = check_setup_values(data)
    data = plot_mem_32ch(data, truncation, signed)
    return data

def within_amp_as_one_ch(fname):
    '''
    Use this when all data saved is supposed to be from the same channel
    in a particular amplifier. In this case, sampling frequency is assumed
    to be 1MHz. Be careful!
    '''
    data = load_mem_values(fname)
    data = convert_mem_values(data)
    data = check_setup_values(data)
    return plot_mem_vals3(data)

def within_amp_1after1(fname, n_channels):
    '''
    Use this when a bunch of data is acquired from 1 channel, then a different
    channel, and so on. Again, samplifing frequency is assumed to be
    1MHz as well, be careful!
    '''
    data = load_mem_values(fname)
    data = convert_mem_values(data)
    data = check_setup_values(data)
    return plot_mem_val4(data, n_channels)



    

    





