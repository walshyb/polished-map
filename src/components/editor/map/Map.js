import Block from './Block';
import { getBlocks } from '../../../utils/wasm-funcs';
import { useAppSelector, useAppDispatch } from '../../../hooks';
import { setBlocks } from '../../../store/editorSlice';
import { getActiveFile } from '../../../store/fileSlice';
import { calculateMapSizes } from '../../../utils/helper-funcs';

export default function Map(
  { fileProcessed }: { fileProcessed: boolean }
) {
  const dispatch = useAppDispatch();
  const blocks = useAppSelector((state) => state.editor.blocks);
  const file = useAppSelector(getActiveFile);
  let width = 0, height = 0;

  if (file) {
    width = calculateMapSizes(file.size).width;
    height = calculateMapSizes(file.size).height;
  }

  let blockGrid = [];

  for (let i = 0; i < width; i++) {
    const row = [];
    for (let j = 0; j < height; j++) {
      const index = i * height + j;
      const block = blocks[index];
      row.push(<Block key={index} id={block.id} />);
    }
    blockGrid.push(
      <div
        key={i}
        className="row"
        style={{ display: 'flex' }}
       >
        {row}
      </div>
    );
  }
  
  return (
    <div>
      {blockGrid}
    </div>
  );
}
