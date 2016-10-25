"" ============================================================================
"" + File name:          vimrc
"" + Description:        Vim startup settings
"" + Author:             leaf
"" ============================================================================
 
"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
" => General
"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
 
set langmenu=en_US      " Sets languages

set encoding=utf-8      " Sets file codings
set termencoding=utf-8
set fileencodings=utf-8,gk2312,gbk,gb18030

set fileformat=unix     " Use Unix line endings
set nocompatible        " Use Vim settings, because IMproved
set confirm             " Ask to save instead of complaining

set hidden              " Don't make a mess or be dumb
set nobackup
set nowritebackup
set noswapfile

set sh=bash             " So vim plays nice with fish
set cmdheight=2         " Avoids 'press <Enter> to continue' ?

set splitbelow          " Splits
set splitright

set history=300         " Sets how many lines of history VIM has to remember
set autoread            " Pick up changes from outside this vim session

" File syntax highlighting and indentation stuff
if !exists("syntax_on")
  syntax on
endif

filetype plugin indent on
autocmd BufNewFile,BufRead *.md set filetype=markdown

" Enable line numbers in printing ?
set printoptions=number:y,left:5pc,top:5pc,right:5pc,bottom:5pc
set timeout ttimeoutlen=50

" Magic for alt keys to work!
let c='a'
while c <= 'z'
  exec "set <A-".c.">=\e".c
  exec "imap \e".c." <A-".c.">"
  let c = nr2char(1+char2nr(c))
endw

"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
" => VIM user interface
"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
colorscheme desert
set t_Co=256                " Set color scheme
set showtabline=2           " Always show the tab line
set tabpagemax=30           " Set the maximum number of tabs

set scrolloff=4             " Avoid edge of the screen
set guicursor=a:blinkon0    " Set no blinking cursor
set number                  " Show line numbers
set wrap                    " No word wrap
set tw=79                   " 88 works better for Pixel

set showcmd
set cmdheight=1             " Command line height
set wildmode=full           " Be as wild as possible
set wildmenu
set wildignore=*.o,*~,*.pyc
set wildignore+=*.class,*.bak
set wildignore+=.git\*,.svn\* 

set synmaxcol=200           " Limit column highlights to 200 columns

set lazyredraw              " Performance!?
set ttyfast

" Colors
set colorcolumn=80          " Handy bar so we know when lines are too long
highlight ColorColumn ctermbg=234

set cursorline              " Handy line so we know where we are
highlight CursorLine cterm=NONE ctermbg=234 ctermfg=NONE
autocmd InsertEnter,InsertLeave * set cul!

highlight Pmenu ctermbg=240
highlight PmenuSel ctermbg=234
highlight PmenuSBar ctermbg=238
highlight PmenuThumb ctermbg=234

" Be less obnoxisous about 'Pattern not found' warnings
highlight Error None
highlight ErrorMsg None

" Searching
set hlsearch                " Highlight searching results
set incsearch
set showmatch
set ignorecase
set ruler                   " Show the cursor position at the status bar

" Search for visually selected text, forwards or backwards!
vnoremap <silent> * :<C-U>
  \let old_reg=getreg('"')<Bar>let old_regtype=getregtype('"')<CR>
  \gvy/<C-R><C-R>=substitute(
  \escape(@", '/\.*$^~['), '\_s\+', '\\_s\\+', 'g')<CR><CR>
  \gV:call setreg('"', old_reg, old_regtype)<CR>
vnoremap <silent> # :<C-U>
  \let old_reg=getreg('"')<Bar>let old_regtype=getregtype('"')<CR>
  \gvy?<C-R><C-R>=substitute(
  \escape(@", '?\.*$^~['), '\_s\+', '\\_s\\+', 'g')<CR><CR>
  \gV:call setreg('"', old_reg, old_regtype)<CR>

" mouse!
function! ToggleMouse()
    " check if mouse is enabled
    if &mouse == 'a'
        " disable mouse
        set mouse=
        echo "Mouse disabled"
    else
        " enable mouse everywhere
        set mouse=a
        echo "Mouse enabled"
    endif
endfunc
map <silent> <leader>b :call ToggleMouse()<cr>
set mouse=a

" scrolling like a GUI!
map <ScrollWheelUp> <C-Y>
map <ScrollWheelDown> <C-E>

" Folding
set foldtext=MyFoldText()
function! MyFoldText()
  let nucolwidth = &fdc + &number*&numberwidth
  let winwd = winwidth(0) - nucolwidth - 5
  let foldlinecount = foldclosedend(v:foldstart) - foldclosed(v:foldstart) + 1
  let prefix = " >>> "
  let fdnfo = prefix . string(v:foldlevel) . "," . string(foldlinecount)
  let line =  strpart(getline(v:foldstart), 0 , winwd - len(fdnfo))
  let fillcharcount = winwd - len(line) - len(fdnfo)
  return line . repeat(" ",fillcharcount) . fdnfo
endfunction
set foldmethod=indent   
set foldnestmax=10
set nofoldenable
set foldlevel=2
set foldcolumn=1

" Auto leave insert mode after 3 seconds of inactivity, hardcore vimmers only
"autocmd CursorHoldI * stopinsert 

" Cool file explorer stuff in a pane
function! ToggleVExplorer()
    if exists("t:expl_buf_num")
        let expl_win_num = bufwinnr(t:expl_buf_num)
        if expl_win_num != -1
            let cur_win_nr = winnr()
            exec expl_win_num . 'wincmd w'
            close
            exec cur_win_nr . 'wincmd w'
            unlet t:expl_buf_num
        else
            unlet t:expl_buf_num
        endif
    else
        exec '1wincmd w'
        Vexplore .
        let t:expl_buf_num = bufnr("%")
    endif
endfunction
nnoremap <silent> <leader>d :call ToggleVExplorer()<CR>

" Hit enter in the file browser to open the selected
" file with :vsplit to the right of browser
let g:netrw_browse_split = 4            " open files in new tab
let g:netrow_altv = 1
let g:netrw_winsize = -28               " thinner width
let g:netrw_sort_sequence = '[\/]$,*'   " directories first
let g:netrw_banner = 0                  " hide the help info
let g:netrw_liststyle = 3               " tree mode

" Clear highlighting when redrawing the screen
nnoremap <C-L> :nohl<CR><C-L>

" Clear junk before running commands
noremap :! :!clear;
noremap :make :!clear; make

" A shell in vim! ctrl-d goes back to vim
noremap <C-d> :sh<cr>

"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
" => Text, tab and indent related
"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
set backspace=indent,eol,start   " allow backspacing over everything in insert
set shiftwidth=2                 " Set 2 spaces for indenting
set tabstop=2                    " Set 2 TAB stops
set softtabstop=2
set expandtab                    " Spaces instead of tabs
set smarttab
set autoindent                   " Set auto indenting on
set smartindent
set nocindent                    " Do not use C language indent style
set showmatch                    " Show matched brackets

set completeopt=menu,menuone
set pumheight=3

" Autocompletion with tab!
function! Tab_Or_Complete()
  if col('.')>1 && strpart( getline('.'), col('.')-2, 3 ) =~ '^\w'
    return "\<C-N>"
  else
    return "\<Tab>"
  endif
endfunction
inoremap <silent> <Tab> <C-R>=Tab_Or_Complete()<CR>

" Auto complete whole lines with ctrl-l !
inoremap <C-L> <C-X><C-L>

autocmd InsertEnter * set noshowmode
autocmd InsertLeave * set showmode

" automatic autocompletion
function! s:skinny_insert(char)
  if !pumvisible() && !exists('s:skinny_complete') && getline('.')[col('.') - 2].a:char =~# '\k\k'
    let s:skinny_complete = 1
      noautocmd call feedkeys("\<C-n>\<C-p>", "nt")
  endif
endfunction

augroup SkinnyAutoComplete
  autocmd!
  autocmd InsertCharPre * silent! call <SID>skinny_insert(v:char)
  autocmd CompleteDone * silent! if exists('s:skinny_complete') | unlet s:skinny_complete | endif
augroup END

"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
" Macros
"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
let g:mapleader = " "

" List macros
nnoremap <leader>? :echo "
      \  : clear highlighting \n
      \+ : toggle dictionary \n
      \1 : jump to tab number \n
      \b : toggle mouse              \| \n
      \C : ! command in pane         \| c : ! command \n
      \d : open in new window        \| \n
      \e : open in new buffer        \| \n
      \H : horizontal split          \| \n
      \i : paste result of command   \| \n
      \j : jump to last buffer       \| \n
      \L : source file               \| l : reload file \n
      \M : make in pane              \| m : make \n
      \N : pwd                       \| n : cycle vim windows \n
      \o : insert blank line         \| \n
      \P : rewrap paragraph          \| p : toggle paste mode \n
      \Q : close all windows         \| q : close window \n
      \R : relative numbering        \| r : run current file \n
      \S : switch windows            \| s : substitute \n
      \t : tab current buffer        \| \n
      \U : source vimrc              \| u : open vimrc \n
      \V : vertical split            \| v : visual block \n
      \w : save                      \| \n
      \x : close file                \| \n
      \y : run in right pane         \| \n
      \Z : toggle spell check        \| z : za\n
      \"<CR>

" clear highlighting
nnoremap <silent> <leader><Space> :nohl<CR>

" super secret encryption algorithm
nnoremap <silent> <leader>G ggg?G<C-o>

" open vimrc
nnoremap <silent> <leader>u :tabe $MYVIMRC<cr>

nnoremap <silent> <leader>U :so $MYVIMRC<cr>

" re hardwrap a paragraph
nnoremap <silent> <leader>P gqip

" toggle paste mode
nnoremap <leader>p :setlocal paste!<cr>:set paste?<cr>

" split the screen vertically and move there
nnoremap <silent> <leader>V <C-w>v<C-w>l

" split the screen horizontally and move there
nnoremap <silent> <leader>H <C-w>s

" close window
nnoremap <silent> <leader>q :q<CR>
nnoremap <silent> <leader>Q :qall<CR>
command! Q :qall

" switch windows
nnoremap <silent> <leader>S <C-w><C-w>
nnoremap <silent> <leader>n <C-w><C-w>

" shell commands
function! Run_Command(command)
  execute "silent !tmux send-keys -t right '" . a:command . "' C-m"
  execute "redraw!"
endfunction
nnoremap <leader>C :call Run_Command('')<left><left>
nnoremap <leader>c :!clear; 

" spell check
nnoremap <silent> <leader>Z :set spell!<CR>

" toggle fold
nnoremap <silent> <leader>z za

" close buffer
nnoremap <silent> <leader>x :bp<bar>sp<bar>bn<bar>bd<CR>

" open file in new buffer
nnoremap <leader>E :e 

" open file in new tab
nnoremap <leader>e :tabe 

" paste result of command
nnoremap <leader>i :r!

" jump to last buffer
nnoremap <silent> <leader>j :w<bar>b#<CR>

" switch to relative numbering
nnoremap <silent> <leader>R :set relativenumber!<CR>

" save the current file
nnoremap <silent> <leader>w :w<CR>
nnoremap <silent> <leader>W :w !sudo tee %<CR>

" insert a blank line below the current line
nnoremap <silent> <leader>o o<Esc>

" make in a tmux pane
nnoremap <silent> <leader>M :!tmux send-keys -t right 
      \ 'make' C-m<cr><cr>

" make
nnoremap <silent> <leader>m :!clear;make<CR>

" open the current buffer in a new tab
nnoremap <silent> <leader>t :tabe %<cr>

" visual block mode
nnoremap <silent> <leader>v <C-S-v>

" generic ':' command
nnoremap <leader>; :
vnoremap <leader>; :

" substitute
vnoremap <leader>s :s/
nnoremap <leader>s V:s/

" reload file
nnoremap <leader>l :e % <cr>

" source file
nnoremap <leader>L :so ~/.vimrc<cr>

" repeat last colon command
nnoremap <leader>. q:k<cr>
vnoremap <leader>. q:k<cr>

" go to tab by number
noremap <leader>1 1gt
noremap <leader>2 2gt
noremap <leader>3 3gt
noremap <leader>4 4gt
noremap <leader>5 5gt
noremap <leader>6 6gt
noremap <leader>7 7gt
noremap <leader>8 8gt
noremap <leader>9 9gt
noremap <leader>0 :tablast<cr>

" running programs in Vim!
" >>>
function! Compile(compile_command)
  if filereadable("./Makefile") || filereadable("./makefile")
    make
  else
    execute "!" . a:compile_command . " " . bufname("%")
  endif
endfunction

autocmd FileType python  nmap <leader>r 
      \ :!clear;python %<cr>
autocmd FileType scheme  nmap <leader>r 
      \ :!clear;racket %<cr>
autocmd FileType sh      nmap <leader>r 
      \ :!clear;bash %<cr>
autocmd FileType perl    nmap <leader>r 
      \ :!clear;perl %<cr>
autocmd FileType haskell nmap <leader>r 
      \ :!clear;runhaskell %<cr>
autocmd FileType java    nmap <leader>r 
      \ :silent call Compile("javac")<cr> <bar>:!clear;java %:r<cr>
autocmd FileType c       nmap <leader>r 
      \ :silent call Compile("gcc")  <cr> <bar>:clear;!./a.out <cr>
autocmd BufNew,BufEnter *.bdot nmap <leader>r 
      \ :!clear;bdgraph %<cr> <bar>:e %<cr>
" <<<

" Run program in pane to the right
" >>>
autocmd FileType python 
      \ nnoremap <leader>y :!tmux send-keys -t right 
      \ 'python %' C-m <cr><cr>
autocmd FileType sh
      \ nnoremap <leader>y :!tmux send-keys -t right 
      \ 'bash %' C-m<cr><cr>
" <<<

" Reload vimrc to fix bugs!
autocmd VimEnter * :so ~/.vimrc

" Search across all open buffers
" >>>
function! BuffersList()
  let all = range(0, bufnr('$'))
  let res = []
  for b in all
    if buflisted(b)
      call add(res, bufname(b))
    endif
  endfor
  return res
endfunction

function! GrepBuffers(expression)
  exec 'vimgrep/'.a:expression.'/ '.join(BuffersList())
endfunction

command! -nargs=+ GrepBufs call GrepBuffers(<q-args>)
nnoremap <leader>/ :GrepBufs 
nnoremap <leader>* 
      \ :call GrepBuffers("<C-R><C-W>")<CR> <bar> :copen 7<cr>
" <<<

" Where are we?
function! CurrentDir()
  :!clear;readlink -f %
endfunction
nnoremap <leader>N :call CurrentDir()<cr>

"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
" Movement
"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""

" When jumping to tags, recenter
nnoremap <silent> <C-]> <C-]>zz

" Move through columns correctly
nnoremap <silent> j gj
nnoremap <silent> k gk

" Switch buffers with parens
noremap <silent> ( :bp<cr>zz
noremap <silent> ) :bn<cr>zz

" Switch tabs with braces
nnoremap <silent> { :tabp<cr>
nnoremap <silent> } :tabn<cr>

" Recenter screen when jumping with search
nnoremap <silent> n nzz
nnoremap <silent> N Nzz

" Faster scrolling Down
nnoremap J 5<C-e>5gj
vnoremap J 5<C-e>5gj
nnoremap <M-j> 4gj
vnoremap <M-j> 4gj
nnoremap <silent> <Down> L5<C-e>5gj

" Map old J to ctrl-j
nnoremap <C-j> J
vnoremap <C-j> J

" Faster scrolling Up
nnoremap K 5<C-y>5gk
vnoremap K 5<C-y>5gk
nnoremap <M-k> 4gk
vnoremap <M-k> 4gk
nnoremap <silent> <Up> H5<C-y>5gk

" Faster repeat last command
nnoremap <silent> <C-@> @@

" Better jump to previous location
nnoremap <silent> <C-o> <C-o>zz

" Even faster jumping around
"nnoremap <silent> D 10<C-y>10gk
"nnoremap <silent> S 10<C-e>10gj

" Faster cycle through quickfix
nnoremap <C-{> :cp<cr>
nnoremap <C-}> :cn<cr>

" Faster move to end and start of lines
nnoremap B ^
vnoremap B ^
nnoremap E $
vnoremap E $

" Faster escape?
inoremap jk <Esc>:w<cr>

" Smarter [Y]anking ? 
map Y y$

" Dictionary!
let g:dict_toggle_is_on = 1
function! DictToggle()
  if g:dict_toggle_is_on
    set dictionary+=/usr/share/dict/words
    set complete+=k
    echo "Dictionary on"
    let g:dict_toggle_is_on = 0
  else
    set dictionary-=/usr/share/dict/words
    set complete-=k
    echo "Dictionary off"
    let g:dict_toggle_is_on = 1
  endif
endfunction
nnoremap <leader>+ :call DictToggle()<CR>

" Horizontal scrolling ?
set scrollopt+=hor
nnoremap <ScrollWheelLeft> :echo HELLO

"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
" Abbreviations
"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
iab newc 
      \#include <stdio.h><cr><cr>
      \int main(int argc, char **argv){<cr>
      \return 0;<cr>
      \}<esc><esc>kko

iab newpy 
      \#!/usr/bin/python<cr><cr>
      \def main()<cr><cr>
      \if __name__ == '__main__':<cr>  main()<esc><esc>:3<cr>A:<cr> 

iab newjava 
      \public class {<cr><cr>
      \public static void main(String[] args) {<cr>}<cr>}
      \<esc><esc>:1<cr>=G<cr>kki

iab sopl System.out.println("");<esc>2hi
iab sepl System.err.println("");<esc>2hi

iab w/ with
iab teh the
iab Its It's
iab its it's
iab hav have
iab ona on a
iab waht what
iab alot a lot
iab THis This
iab THat That
iab dont don't
iab wont won't
iab diong doing
iab dont' don't
iab wont' won't
iab prety pretty
iab realy really
iab whould would
iab shoudl should
iab useable usable
iab recieve receive
iab similiar similar
iab tomorow tomorrow
iab shouldnt shouldn't
iab finacial financial
iab somethign something
iab diference difference
iab opprotunity opportunity
iab occasionaly occasionally
iab explainations explanations
