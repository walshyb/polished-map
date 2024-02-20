import Block from './Block';
import { getBlocks } from '../../../utils/wasm-funcs';
import { useAppSelector, useAppDispatch } from '../../../hooks';
import { setBlocks } from '../../../store/editorSlice';
import { getActiveFile } from '../../../store/fileSlice';

export default function Map(
  { fileProcessed }: { fileProcessed: boolean }
) {

  const dispatch = useAppDispatch();
  const blocks = useAppSelector((state) => state.editor.blocks);
  const file = useAppSelector(getActiveFile);
  console.log('active file', file);
  const rows = 18;
  const cols = 20;


  
  return (
    <div>
    </div>
  );
}
